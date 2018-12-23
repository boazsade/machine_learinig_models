#include "csv_fast_parser.h"
#include "csv_parser_details.hpp"
#include <iostream>

namespace csv
{

void free_impl(detail::csv_driver* ptr) { delete ptr; }

fast_parser::fast_parser(char delim) : parser_impl(new detail::csv_driver(delim), free_impl)
{
}

bool fast_parser::eof() const
{
    return parser_impl && parser_impl->eof();
}

bool fast_parser::error() const
{
    return !parser_impl || parser_impl->error();
}

bool fast_parser::good() const
{
    return parser_impl && parser_impl->good();
}

bool fast_parser::eol() const
{
    return parser_impl && parser_impl->end_record();
}

int fast_parser::process(source_type& in_stream)
{
    if (!in_stream.bad()) {
        return in_stream.eof() ? 0 : 1; // not really mater
    } else {
        return -1;
    }
}

fast_parser::token_type fast_parser::get_token(source_type& in_stream)
{
    if (process(in_stream) < 0 || !parser_impl) {
        static constexpr auto err = token_type{};
        return err;
    } else {
        auto c = parser_impl->process(in_stream);
        if (c.valid()) {
            return token_type{c.begin(), (token_type::size_type)c.size()};
        } else {
            return token_type{};
        }
    }
}

fast_parser::token_type fast_parser::get_token()
{
    if (parser_impl && parser_impl->has()) {
        auto c = parser_impl->get_current();
        return token_type{c.begin(), (token_type::size_type)c.size()};
    } else {
        static const token_type empty = token_type{};
        return empty;
    }
}

std::string to_string(const tepmorary_token& token)
{
    if (!token.empty()) {
        return std::string{token.start, token.length};
    }
    static const std::string empty = std::string{};
    return empty;
}


std::ostream& operator << (std::ostream& os, const tepmorary_token& t)
{
    return os<<to_string(t);
}

}   // end of namespace csv

