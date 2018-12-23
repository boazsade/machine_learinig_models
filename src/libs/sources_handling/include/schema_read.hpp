#pragma once
#include "schema_type.h"
#include <boost/tokenizer.hpp>
#include <iostream>

namespace sr
{

template<typename F>
schema_type read_schema(std::istream& input, const char* delim, F func) 
{
    std::string line;
    if (input && std::getline(input, line)) {
        schema_type schema;
        const boost::char_separator<char> sep(delim);
        boost::tokenizer<boost::char_separator<char>> tok(line, sep);
        std::transform(std::begin(tok), std::end(tok), std::back_inserter(schema), func);
        return schema;
    }
    return schema_type{};   // error case
}

}   // end of namespace sr


