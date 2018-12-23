#include "libs/sources_handling/row_data_input.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/schema_read.hpp"
#include "libs/ml_models/utilities.h"
#include "libs/system/exceptions.h"
#include "utils/missing_values.hpp"
#include "logging.h"
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <iostream>
#include <random>

namespace
{

    template<typename T, typename S>
    struct cast_it
    {
        static constexpr T apply(S from)
        {
            return static_cast<T>(from);
        }
    };

    template<typename T>
    struct cast_it<T, T>
    {
        static constexpr T apply(T from)
        {
            return from;
        }
    };

    template<typename T, typename S>
    T cast_value(S from)
    {
        using cast_op = cast_it<T, S>;
        return cast_op::apply(from);
    }

    template<typename T, bool F>
    struct validation 
    {
        template<typename S>
        static T convert(S from, const std::string&) {
            return cast_value<S>(from);
        }
    };

    template<typename T>
    struct validation<T, true>
    {
        template<typename S>
        static T convert(S from, const std::string& cid) {
            if (invalid_value(from) || missing_value(from)) {
                throw malformat_input{cid};
            }
            return cast_value<S>(from);
        }
    };

    template<typename TV, typename SV, typename S> inline
    TV from_token(S&& from, const std::string& cid) {
        using validator = validation<SV, std::is_floating_point<SV>::value>;

        return validator::convert(boost::lexical_cast<SV>(from), cid);
    }

    unsigned int process_line(const std::string& line, classifier::read_result& ret)
    {
        using tokenizer = boost::tokenizer<boost::char_separator<char>>;
        using target_value = classifier::read_result::value_type::data_type::value_type;
        using source_value = source_data::algo_data;
        static const boost::char_separator<char> sep(",");

        tokenizer tokens(line, sep);
        unsigned int index = 0;
        for (auto& token: tokens) {
            if (index < ret.size()) {
                const auto& id = ret[index].ID;
                ret[index++].data.push_back(
                        from_token<target_value, source_value>(token, id)
                        //cast_value<target_value>(boost::lexical_cast<source_value>(token))
                    );
            }
        }
        return index;
    }

    bool read_entries(std::istream& from, classifier::read_result& ret)
    {
        // note read each line convert to the entry
        auto index = 0;
        while (from) {
            std::string line;
            if (std::getline(from, line)) {
                auto r = process_line(line, ret);
                if (r !=  ret.size()) {
                    if (r == 0u && !ret[0].data.empty()) {
                        return true;    // we are at the end but we did read input
                    }
                    LOG_ERROR<<"failed to process line number "<<index<<" expected value "<<ret.size()<<" got "<<r;
                    return false;
                }
                ++index;
            }
        }
        LOG_INFO<<"successfully read "<<ret.size()<<" columns and "<<ret[0].data.size()<<" rows";
        return !ret[0].data.empty();
    }

}   // end of local namespace

column_data::column_data(const id_type& id) :
    ID(id)
{
}

std::ostream& operator << (std::ostream& os, 
        const column_data& cd)
{
    return os<<"ID: "<<cd.ID<<", data size "<<cd.data.size();
}

schema_type read_schema(std::istream& from)
{
    const auto process_trim_token = [] (auto token) { 
        if (token.size() < 3) { 
            throw std::runtime_error{"invalid token read - too short"};
        }
        return std::string{&token[1], token.size()-2};   // trim start/end
    };

    return sr::read_schema(from, ",", process_trim_token);
}

namespace classifier
{

namespace details
{
void shuffle(std::size_t rn, read_result::iterator be_data,
        read_result::iterator end_data);
void shuffle_rows(int from_inx, int to_inx, 
        read_result::iterator be_data,
        read_result::iterator end_data);
std::tuple<read_result::value_type::data_type::const_iterator,
           read_result::value_type::data_type::const_iterator>
    prepare4labels_copy(const read_result& data,
                const column_data::id_type& id,
                unsigned int rows, unsigned int off);
void shuffle_row(int from_inx, int to_inx, 
        read_result::value_type::data_type::iterator begin);

void shuffle(std::size_t rn, read_result::iterator be_data,
        read_result::iterator end_data)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, (int)rn);
    for (auto i = 0u; i < rn; ++i) {
        shuffle_rows(distr(eng), distr(eng),
                     be_data, end_data);
    }
}

void shuffle_row(int from_inx, int to_inx, 
        read_result::value_type::data_type::iterator begin)
{
    auto fi = begin + from_inx;
    auto ft = begin + to_inx;
    //std::cout<<"swaping "<<*ft<<" and "<<*fi<<", ";
    std::swap(*fi, *ft);
    
}

void shuffle_rows(int from_inx, int to_inx, 
        read_result::iterator be_data,
        read_result::iterator end_data)
{
    //std::cout<<"trying to swap at location "<<from_inx<<" and "<<to_inx<<std::endl;
    if (from_inx != to_inx) {
        std::for_each(be_data, end_data, [from_inx, to_inx](auto& col) {
                    shuffle_row(from_inx, to_inx, col.data.begin());
                }
        );
    }
}

std::tuple<read_result::value_type::data_type::const_iterator,
           read_result::value_type::data_type::const_iterator>
    prepare4labels_copy(const read_result& data,
                const column_data::id_type& id,
                unsigned int rows, unsigned int off)
{
    using iterator = read_result::value_type::data_type::const_iterator;
    if (empty(data)) {
        return std::make_tuple(iterator{}, iterator{});
    }
    auto i = std::find_if(std::begin(data), std::end(data), 
                                [&id] (auto& col) {
                                    return col.ID == id;
                                }
    );
    if (i == std::end(data)) {
        return std::make_tuple(iterator{}, iterator{});
    } else {
        if ((off + rows) > i->data.size()) {    // make sure not to overflow!!
            rows -= (off + rows) - i->data.size();
        }
        rows = std::min(rows, (unsigned int)i->data.size());
        mlmodels::class_data ret(rows, 0.f);
        auto begin = std::begin(i->data) + off;
        auto end = begin + rows;
        return std::make_tuple(begin, end);
    }

}

}

read_result read(std::istream& from, const schema_type& schema)
{
    if (from && !schema.empty()) {
        LOG_INFO<<"reading data based on schema with "<<schema.size()<<" columns";
        read_result ret;
        std::for_each(std::begin(schema), std::end(schema), [&ret](auto& cn) {
                ret.emplace_back(cn);
            }
        );
        if (read_entries(from, ret)) {
            return ret;
        } // just fall to the default - which would signal an error
    } else {
        LOG_ERROR<<"failed to read - "<<(from ? "schema empty" : "source file is invalid");
    }
    return read_result{};
}

mlmodels::training_data data_matrix(const read_result& row_data, 
        const column_data::id_type& label_col, 
        unsigned int rows_count, unsigned int offset)
{
    static const std::string index_column = "index";
    // we need to build a matrix that would be transpose of the
    // data we have and without the data from the column that
    // is marked as label column, we also need to stop after
    // the given number of rows
    if (empty(row_data)) {
        return mlmodels::training_data{}; // invalid input
    }
    
    rows_count = std::min(rows_count, (unsigned int)rows(row_data));
    // build list of columns that we would save data from
    std::vector<read_result::const_iterator> indices;
    for (auto it = row_data.begin(); it !=  row_data.end(); ++it) {
        if (it->ID != label_col && it->ID != index_column) {
            indices.push_back(it);
        }
    }
    if ((rows_count + offset) > rows(row_data)) { // make sure not to overflow!!
        rows_count -= (rows_count + offset) - rows(row_data);
    }
    mlmodels::training_data result(boost::extents[rows_count][indices.size()]);
    // copy the number of rows that the user wants
    for (auto i = 0u; i < rows_count; ++i, ++offset) {
        auto j = 0u;
        for (const auto& cit : indices) {
            result[i][j++] = cit->data[offset];
        }
    }
    return result;
}

mlmodels::class_data labels_data(const read_result& row_data,
        const column_data::id_type& label_col, 
        unsigned int rows, unsigned int off)
{
    auto r = details::prepare4labels_copy(row_data, label_col, rows, off);
    auto begin = std::get<0>(r);
    auto end = std::get<1>(r);
    if (begin == end) {
        return mlmodels::class_data{};    // invalid call
    }
    mlmodels::class_data ret(std::distance(begin, end), 0.f);
    std::copy(begin, end, std::begin(ret));
    return ret;
}

mlmodels::classes_data transform2labels(const read_result& from,
                    const column_data::id_type& label_col,
                    unsigned int rows, unsigned int off)
{
    auto r = details::prepare4labels_copy(from, label_col, rows, off);
    auto begin = std::get<0>(r);
    auto end = std::get<1>(r);
    if (begin != end) {
        return mlmodels::classes_transform(begin, end);
    } else {
        return mlmodels::classes_data{};
    }
}

bool empty(const read_result& rr)
{
    return rr.empty() || rr[0].data.empty();
}

std::size_t rows(const read_result& rr)
{
    return empty(rr) ? 0u : rr[0].data.size();
}

std::size_t columns(const read_result& rr)
{
   return rr.size();
}

const read_result::value_type::data_type&
    data_entry(const read_result& from, const column_data::id_type& id)
{
    auto i = std::find_if(std::begin(from), std::end(from), [&id](auto& c) {
                            return c.ID == id;
                        }
                );
    if (i == std::end(from)) {
        throw std::out_of_range{"column " + id + " do not exists"};
    } 
    return i->data;
}

read_result&& randomalized(read_result from)
{
    if (empty(from)) {
        return std::move(from);
    }
    auto rn = rows(from) - 1u;

    details::shuffle(rn, from.begin(), from.end());
    return std::move(from);
}

}   // end of namespace classifer

