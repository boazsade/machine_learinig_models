#include "libs/cleanup_operatiosn/schema_data.h"
#include <boost/tokenizer.hpp>
#include <fstream>
#include <algorithm>
#include <iterator>


schema_type get_schema(std::istream& input)
{
    static const std::string ignore_entry = "index";
    static const boost::char_separator<char> sep(" \t");
    std::string line;

    if (std::getline(input, line)) {  // the first line contains all field names
        schema_type result;
        boost::tokenizer<boost::char_separator<char>> tok(line, sep);
        std::copy(tok.begin(), tok.end(), std::back_inserter(result)); 
        auto i = std::find(std::begin(result), std::end(result), ignore_entry);
        result.erase(i);
        return result;
    }
    throw std::runtime_error{"failed to read schema"};
}