#include "test_number.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_real.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <type_traits>
#include <iostream>

template<unsigned>
struct parse_int;

template<unsigned int Radix>
struct parse_int_base {
    boost::spirit::qi::int_parser<long long, Radix> parser;
};

template<>
struct parse_int<10> : private parse_int_base<10>
{
    template<typename It>
    bool run(It b, It e) const 
    {
        long long result = 0;
        return (boost::spirit::qi::parse(b, e, parser, result) && b == e);
    }
};

template<>
struct parse_int<8> : private parse_int_base<8>
{
    template<typename It>
    bool run(It b, It e) const 
    {
        long long result = 0;
        return (boost::spirit::qi::parse(b, e, parser, result) && b == e);
    }
};

template<>
struct parse_int<16> : private parse_int_base<16>
{
    template<typename It>
    bool run(It b, It e) const 
    {
        long long result = 0;
        return (boost::spirit::qi::parse(b, e, parser, result) && b == e);
    }
};

struct parse_real {
    template<typename It>
    bool operator () (It from, It to) const
    {
        long double d = 0.0;
        return boost::spirit::qi::parse(from, to, parser, d) && from == to;
    }

private:
    using parser_type = boost::spirit::qi::real_parser<long double>;
    parser_type parser;

};

//template <typename RetType = int, unsigned Radix = 10, typename Parser = typename boost::spirit::qi::int_parser<RetType, Radix> >
template <unsigned Radix = 10, typename It = std::string::const_iterator>
bool is_integer(It from, It to)
{
    using parser_type = parse_int<Radix>;

    static const parser_type parser = parse_int<Radix>{};

    return  parser.run(from, to);

}
#if 0
int redix_type(const char* type) {
    static const std::string types[] = {
        std::string{"dec"}, std::string{"hex"}, std::string{"oct"}
    };
    if (type == types[0]) {
        return 10;
    } else if (type == types[1]) {
        return 16;
    } else if (type == types[2]) {
        return 8;
    } else {
        return 10;
    }
}
#endif
int guess_type(const std::string& str, std::string::size_type& offset) {

    if (str.size() > 1 && str[0] == '0' && std::isdigit(str[1])) {  // maybe octal
        offset = 1;
        return 8;
    }
    if (str.size() > 2 && str[0] == '0') {
        if (str[1] == 'x' || str[1] == 'X') {
            offset = 2;
            return 16;
        }
    }
    return -1;
}

bool predicte(std::string str)
{
    std::string::size_type off = 0;
    int t = guess_type(str, off);
    if (t != -1) {
        switch (t) {
        case 8:
            return is_integer<8>(str.cbegin()+off, str.cend());
        case 16:
            return is_integer<16>(str.cbegin() + off, str.cend());
        default:
            break;
        }
    }
    // we don't know what it is, so just guess
    return is_integer<10>(str.cbegin(), str.cend()) || is_integer<16>(str.cbegin(), str.cend())
        || is_integer<8>(str.cbegin(), str.cend());
}

bool is_number(std::string str)
{
    static const parse_real pr = parse_real{};

    boost::algorithm::trim(str);    // remove any leading and trailing spaces - so we would only work on whats mater
    if (str.empty()) {
        return false;
    }
    return predicte(str) || pr(str.cbegin(), str.cend());
}

