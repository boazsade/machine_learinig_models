#include "row_data_input.h"
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <iostream>

namespace
{

    unsigned int process_line(const std::string& line, classifier::read_result& ret)
    {
        using tokenizer = boost::tokenizer<boost::char_separator<char>>;
        using target_value = classifier::read_result::value_type::data_type::value_type;
        static const boost::char_separator<char> sep(",");

        tokenizer tokens(line, sep);
        unsigned int index = 0;
        for (auto& token: tokens) {
            if (index < ret.size()) {
                ret[index++].data.push_back(boost::lexical_cast<target_value>(token));
            }
        }
        return index;
    }

    bool read_entries(std::istream& from, classifier::read_result& ret)
    {
        // note read each line convert to the entry
        while (from) {
            std::string line;
            if (std::getline(from, line)) {
                if (process_line(line, ret) !=  ret.size()) {
                    return false;
                }
            }
        }
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

read_result read(std::istream& from, const schema_type& schema)
{
    if (from && !schema.empty()) {
        read_result ret;
        std::for_each(std::begin(schema), std::end(schema), [&ret](auto& cn) {
                ret.emplace_back(cn);
            }
        );
        if (read_entries(from, ret)) {
            return ret;
        } // just fall to the default - which would signal an error
    }
    return read_result{};
}

training_data data_matrix(const read_result& row_data, 
        const column_data::id_type& label_col, unsigned int rows)
{
    // we need to build a matrix that would be transpose of the
    // data we have and without the data from the column that
    // is marked as label column, we also need to stop after
    // the given number of rows
    if (row_data.empty() || row_data[0].data.empty()) {
        return training_data{}; // invalid input
    }
    rows = std::min(rows, (unsigned int)row_data[0].data.size());
    training_data result(row_data.size() - 1u, column_data::data_type(rows, 0.f));
    // build list of columns that we would save data from
    std::vector<read_result::const_iterator> indices;
    for (auto it = row_data.begin(); it !=  row_data.end(); ++it) {
        if (it->ID != label_col) {
            indices.push_back(it);
        }
    }
    // copy the number of rows that the user wants
    for (auto i = 0u; i < rows; ++i) {
        auto j = 0u;
        std::for_each(std::begin(indices), std::end(indices), [&result, i, &j] (auto ri) {
                result[j][i] = ri->data[i];
            }
        );
    }
    return result;
}

class_data labels_data(const read_result& row_data,
        const column_data::id_type& label_col, unsigned int rows)
{
    if (row_data.empty() || row_data[0].data.empty()) {
        return class_data{};    // invalid call
    }
    auto i = std::find_if(std::begin(row_data), std::end(row_data), [&label_col] (auto& col) {
                            return col.ID == label_col;
                        }
    );
    if (i == std::end(row_data)) {
        return class_data{};    // error
    } else {
        rows = std::min(rows, (unsigned int)i->data.size());
        class_data ret(rows, 0.f);
        auto end = std::begin(i->data) + rows;
        std::copy(std::begin(i->data), end, std::begin(ret));
        return ret;
    }
}

}   // end of namespace classifer

