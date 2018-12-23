#pragma once
#include "libs/csv_parser/csv_stream.h"
#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <iosfwd>
#include <string>
#include <memory>

namespace csv
{

namespace detail
{
    struct csv_driver;
}
// fast parser means that we don't expect to hadnle anything other then 
// fields delimit with some delimiters. We would not have new lines and quetes inside a 
// line. so in this case we would simply read from the input one line at a time
// proces it by calling boost tokenizer that would split the input into tokens
// until we would complete the parsing. please note that this parser is lazy
// so that you would need to feed it with the stream, but it would read from the stream
// one line the time
struct tepmorary_token
{
    using const_pointer = const char*;
    using size_type = unsigned short;   // we don't expect token to be more than 65000 chars

    tepmorary_token() = default;
    tepmorary_token(tepmorary_token&&) = default;
    tepmorary_token& operator = (tepmorary_token&&) =default;
    tepmorary_token(const tepmorary_token&) = default;
    tepmorary_token& operator = (const tepmorary_token&) =  default;

    constexpr tepmorary_token(const_pointer be, size_type len) : start(be), length(len)
    {
    }

    constexpr bool empty() const
    {
        return !start || length == 0;
    }

    const_pointer start = nullptr;
    size_type     length = 0;
};

std::string to_string(const tepmorary_token& from);

std::ostream& operator << (std::ostream& os, const tepmorary_token& t);

template<typename Target>
inline Target cast(const tepmorary_token& from)
{
    //return boost::lexical_cast<Target>(to_string(from));
    boost::cnv::spirit converter;
    return boost::convert<Target>(to_string(from), converter).value();
}

template<typename Target, typename Convert>
inline Target cast(const tepmorary_token& from, Convert convert)
{
    return convert(to_string(from));    // user give the convertion function - if user knows best
} 

struct fast_parser
{
    using source_type = string_buffer<std::string::value_type>;
    using token_type = tepmorary_token;
    using boolean_type = bool (fast_parser::*)() const;
    //using source_type = data_stream<std::istream>;
    //using token_type = std::string;
    static constexpr char default_delimiter = ',';

    explicit fast_parser(char delim = default_delimiter);

    fast_parser(const fast_parser&) = delete;
    fast_parser& operator = (const fast_parser&) = delete;

    int process(source_type& in_stream);    // return the number of extracted chars or -1 on error

    bool eof() const;   // return true if no more input

    bool error() const;

    bool good() const;  // return true if we don't have error

    bool eol() const;   // return true of the line ended

    operator boolean_type () const
    {
        return good() ? &fast_parser::eol : (boolean_type)nullptr;
    }

    token_type get_token();

    token_type get_token(source_type& in_stream);   // this would combine get_token() and process(stream)

private:

    enum state { INIT = 0, GOOD, BAD, END };
    //std::string::size_type current_token = std::string::npos;
    state current_state = INIT;
    std::unique_ptr<detail::csv_driver, void(*)(detail::csv_driver*)> parser_impl;

};

}   // end of namespace csv

