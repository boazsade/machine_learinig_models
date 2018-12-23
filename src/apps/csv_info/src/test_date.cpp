#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <boost/algorithm/string/trim.hpp>
//#include <regex>

auto constexpr iso8601_min_year = 1583;
auto constexpr max_month = 12;
auto constexpr max_day = 31;

bool check_days_in_manth(int day, int month, int year)
{
    static constexpr auto year_fact = 400;
    static constexpr auto year_dis_fact = 100;
    static constexpr auto year_dis_fact2 = 4;
    static const int months[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    if (months[month] >= day) {
        return true;
    } else {
        if (month == 1) {
            if (year % year_fact == 0 || (year % year_dis_fact != 0 && year % year_dis_fact2 == 0)) {
                return day <= months[month]+1;
            }
        }
    }
    return false;
}

bool check_values(int year, int month, int day)
{

    if (year < iso8601_min_year) {
        return false;
    }

    if (month < 1 || month > max_month) {
        return false;
    } else {
        --month;
    }
    if (day < 1 || day > max_day) {
        return false;
    }
    return check_days_in_manth(day, month, year);
}

bool none_iso8601_match(const char* str) {
    int year = -1, month = -1, day = -1;
    // in this case we don't know the actual order..
    static const char* patterns[] = {
        "%d/%d/%d", "%d:%d:%d"
    };
    for (auto i = std::begin(patterns); i != std::end(patterns); i++) {
        auto r = sscanf(str, *i, &year, &month, &day);
        if (r == 3) {
            if (month > max_month && day <= max_month) {
                std::swap(month, day);
            }
            if (check_values(year, month, day)) {
                return true;
            }
        }
    }
    return false;
}

bool iso8601_submatch(const char* str)
{
    int year = -1, month = -1, day = -1;
    auto r = sscanf(str, "%4d%2d%2d", &year, &month, &day);
    switch (r) {
        case 1:
            return false;
        case 2:
            return check_values(year, month, 0);
        case 3:
            return check_values(year, month, day);
        default:
            return false;
    }
}

bool iso8601_match(const char* str) 
{
    int year = -1, month = -1, day = -1;
    auto r = sscanf(str, "%4d-%2d-%2d", &year, &month, &day);  // ISO format yyyy-mm-dd
    switch (r) {
        case 1: // only year, this may mean we have no delimiter
            return iso8601_submatch(str);
        case 2:
            return check_values(year, month, 0);
        case 3:
            return check_values(year, month, day);
        default:
            return iso8601_submatch(str);
    }
}
#if 0
bool regex_iso8601(const std::string& str) {
    static const std::regex matcher("^(\\d{4}(?:(?:(?:\\-)?(?:00[1-9]|0[1-9][0-9]|[1-2][0-9][0-9]|3[0-5][0-9]|36[0-6]))?|(?:(?:\\-)?(?:1[0-2]|0[1-9]))?|(?:(?:\\-)?(?:1[0-2]|0[1-9])(?:\\-)?(?:0[1-9]|[12][0-9]|3[01]))?|(?:(?:\\-)?W(?:0[1-9]|[1-4][0-9]5[0-3]))?|(?:(?:\\-)?W(?:0[1-9]|[1-4][0-9]5[0-3])(?:\\-)?[1-7])?)?)$");
    if (std::regex_search(str, matcher)) {
        return true;
    }
    return false;
}
#endif
bool is_date(std::string input) {
    boost::algorithm::trim(input);    
    if (input.empty()) {
        return false;
    }
    return iso8601_match(input.c_str()) || none_iso8601_match(input.c_str());
}

