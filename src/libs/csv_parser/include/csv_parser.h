#pragma once
#include "libs/csv_parser/csv_buffer.h"
#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <string>
#include <iosfwd>
#include <cstdlib>

namespace csv
{

// to parse CSV successfully we need to know 2 things
// 1. What is the delimiter (which default to ',')
// 2. escape char (which defaults to '"')
// once have those two we can iterete over a stream of input chars
// and find each of the cells in the row. the row would end once we have
// a new line that is not inside an escaped string
// note - this parser is statefull in the sence that we are passing a stream to it
// but it would stop at each token and return it to the caller. 
// There can be there options - 
// 1. either it found a token, 
// 2. it found a token and end of row
// 3. it found a token and end of file
// in addition, it may have an error, but then it would not be able to return the 
// token back
class parser
{
public:
    static constexpr char default_delimiter = ',';
    static constexpr char default_escape = '"';
    static constexpr char default_escape_escape = '\\';
    static constexpr int buffer_size = 1024;
    
    enum state {
        INIT = 0,
        START_TOKEN,
        IN_TOKEN,
        MAY_ESCAPE,
        MAY_ESCAPE_END,
        IN_ESCAPE,
        END_ESCAPE_IN_TOKEN,
        ESCAPE_OF_ESCAPE,
        END_TOKEN,
        END_LINE,
        END_INPUT,
        ERROR
    };

    using token_type = std::string;
    using source_type = source<std::istream, buffer_size>;

    explicit parser(char delim = default_delimiter, char escape = default_escape, char escape_escape = default_escape_escape);
    parser(parser&&) = default;
    parser(const parser&) = default;
    parser& operator = (parser&&) = default;
    parser& operator = (const parser&) = default;

    bool error() const;

    bool good() const;

    bool has_token() const;

    bool eol() const;                   // return true we we found end of line (that is a valid end of line that is not inside escape)

    bool eof() const;

    bool get(std::string& new_token);  // note that it would return empty string if we didn't parse or that we have an error

    std::string get_token();

    std::string get_token(source_type& from);

    int process(source_type& from);    // return the number of bytes it processed, or -1 on error. this would stop once we have a token ready or we have error
    
private:
    bool internal_process(char in);
    bool internal_process2(char in);
private:

    token_type token;
    char delimiter = default_delimiter;
    char escape_char = default_escape;
    char escape_escape_char = default_escape_escape;
    state current_state = INIT;
};

namespace detail
{
    template<typename T>
    struct convert
    {
        static T apply(const std::string& from)
        {
            boost::cnv::spirit converter;
            return boost::convert<T>(from, converter).value();
        }
    };

    template<>
    struct convert<float>
    {
        static float apply(const std::string& from)
        {
            return std::strtof(from.c_str(), nullptr);
        }
    };

    template<>
    struct convert<double>
    {
        static double apply(const std::string& from)
        {
            return std::strtod(from.c_str(), nullptr);
        }
    };

    template<>
    struct convert<long double>
    {
        static long double apply(const std::string& from)
        {
            return std::strtold(from.c_str(), nullptr);
        }
    };

    template<>
    struct convert<int>
    {
        static int apply(const std::string& from)
        {
            return std::atoi(from.c_str());
        }
    };

    template<>
    struct convert<long>
    {
        static long apply(const std::string& from)
        {
            return std::strtol(from.c_str(), nullptr, 10);
        }
    };

    template<>
    struct convert<long long>
    {
        static long long apply(const std::string& from)
        {
            return std::strtoll(from.c_str(), nullptr, 10);
        }
    };

    template<>
    struct convert<unsigned long long>
    {
        static unsigned long long apply(const std::string& from)
        {
            return std::strtoull(from.c_str(), nullptr, 10);
        }
    };

    template<>
    struct convert<unsigned long>
    {
        static unsigned long apply(const std::string& from)
        {
            return std::strtoul(from.c_str(), nullptr, 10);
        }
    };

    template<>
    struct convert<unsigned int>
    {
        static unsigned int apply(const std::string& from)
        {
            return (unsigned int)std::atoi(from.c_str());
        }
    };

    template<>
    struct convert<unsigned short>
    {
        static unsigned short apply(const std::string& from)
        {
            return (unsigned short)convert<unsigned short>::apply(from);
        }
    };

    template<>
    struct convert<short>
    {
        static short apply(const std::string& from)
        {
            return (short)convert<short>::apply(from);
        }
    };

    template<>
    struct convert<char>
    {
        static char apply(const std::string& from)
        {
            return from.empty() ? '\0' : from[0];
        }
    };

    template<>
    struct convert<unsigned char>
    {
        static unsigned char apply(const std::string& from)
        {
            return from.empty() ? (unsigned char)0xff : (unsigned char)from[0];
        }
    };


    template<>
    struct convert<std::string>
    {
        static const std::string& apply(const std::string& s)
        {
            return s;
        }
    };
}   // end of namespace detail

template<typename Result> inline
Result cast(const std::string& from)
{
    return detail::convert<Result>::apply(from);
}

}   // end of namespace csv

