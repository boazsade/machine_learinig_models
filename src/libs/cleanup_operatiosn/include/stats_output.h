#pragma once
#include "utils/math_datatype.h"
#include <string>
#include <vector>
#include <iosfwd>

struct column_info
{
    using min_max_t = math::min_max<double>; 
    using stats_t = math::statistical_data<double>; 

    constexpr column_info() = default;

    constexpr column_info(const min_max_t& mm,
                        const stats_t& s) : 
                range(mm), stats(s)
    {
    }

    min_max_t range;
    stats_t stats;
};  

constexpr inline bool 
good(const column_info& cf) {
    return math::good(cf.stats) &&
        math::good(cf.range);
}

std::ostream& operator << (std::ostream& os, const column_info& se);

struct stats_output
{
    stats_output() = default;

    stats_output(const std::string& n, const column_info& ci);

    column_info info;
    std::string name;
};
std::ostream& operator << (std::ostream& os, const stats_output& so);
stats_output make_stats(const std::string& s, const column_info& ci);

using all_stats = std::vector<stats_output>;
