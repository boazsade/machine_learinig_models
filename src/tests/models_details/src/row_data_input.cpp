#include "apps/run_experiments/row_data_input.h"
#include "apps/data_clean/source_data.h"
#include "libs/classfication_algos/utilities.h"
#include "logging.h"
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <iostream>

namespace
{

    template<typename T, typename S>
    struct cast_it
    {
        static T apply(S from)
        {
            return static_cast<T>(from);
        }
    };

    template<typename T>
    struct cast_it<T, T>
    {
        static T apply(T from)
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
                ret[index++].data.push_back(
                        cast_value<target_value>(boost::lexical_cast<source_value>(token))
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
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
            
    std::string input;
    if (from && std::getline(from, input)) {
        schema_type schema;
        boost::char_separator<char> sep(",");
        tokenizer tokens(input, sep);
        std::transform(std::begin(tokens), std::end(tokens), std::back_inserter(schema), [](auto token) {
                    if (token.size() < 3) { // we are expecting to have a token with format "token" without the quate its invalid
                        throw std::runtime_error{"invalid input read for schema column names"};
                    }
                    std::string ret{&token[1], token.size()-2}; // remove leading and trailing " chars
                    return ret;
                }
        );
        return schema;
    } else {
        throw std::runtime_error{"failed to read shcema for data input"};
    }
}

namespace classifier
{

namespace details
{
std::tuple<read_result::value_type::data_type::const_iterator,
           read_result::value_type::data_type::const_iterator>
    prepare4labels_copy(const read_result& data,
                const column_data::id_type& id,
                unsigned int rows, unsigned int off);

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
        class_data ret(rows, 0.f);
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

training_data data_matrix(const read_result& row_data, 
        const column_data::id_type& label_col, 
        unsigned int rows_count, unsigned int offset)
{
    static const std::string index_column = "index";
    // we need to build a matrix that would be transpose of the
    // data we have and without the data from the column that
    // is marked as label column, we also need to stop after
    // the given number of rows
    if (empty(row_data)) {
        return training_data{}; // invalid input
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
    training_data result(boost::extents[rows_count][indices.size()]);
    // copy the number of rows that the user wants
    for (auto i = 0u; i < rows_count; ++i, ++offset) {
        auto j = 0u;
        for (const auto& cit : indices) {
            result[i][j++] = cit->data[offset];
        }
    }
    return result;
}

class_data labels_data(const read_result& row_data,
        const column_data::id_type& label_col, 
        unsigned int rows, unsigned int off)
{
    auto r = details::prepare4labels_copy(row_data, label_col, rows, off);
    auto begin = std::get<0>(r);
    auto end = std::get<1>(r);
    if (begin == end) {
        return class_data{};    // invalid call
    }
    class_data ret(std::distance(begin, end), 0.f);
    std::copy(begin, end, std::begin(ret));
    return ret;
}

classes_data transform2labels(const read_result& from,
                    const column_data::id_type& label_col,
                    unsigned int rows, unsigned int off)
{
    auto r = details::prepare4labels_copy(from, label_col, rows, off);
    auto begin = std::get<0>(r);
    auto end = std::get<1>(r);
    if (begin != end) {
        return classes_transform(begin, end);
    } else {
        return classes_data{};
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

}   // end of namespace classifer

