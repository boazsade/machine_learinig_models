#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/stats_output.h"
#include "utils/math_datatype_io.h"
#include <iostream>


std::ostream& operator << (std::ostream& os, const column_info& se)
{
    return os<<"column value range: "
        <<se.range
        <<", stats: "
        <<se.stats;
}

stats_output::stats_output(const std::string& n, const column_info& ci) :
        info(ci), name(n)
{
}

std::ostream& operator << (std::ostream& os, const stats_output& so)
{
    return os<<"column: "<<so.name<<", stats: "<<so.info;
}

stats_output make_stats(const std::string& s, const column_info& ci)
{
    return stats_output{s, ci};
}

