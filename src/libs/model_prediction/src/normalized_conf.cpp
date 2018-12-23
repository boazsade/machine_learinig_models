#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif  // SUPPORT_FOR_MODEL_USE
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "details/base_normalized_conf.hpp"
#include "utils/math_datatype_io.h"
#include "utils/math_datatype.h"
#include <iostream>

std::ostream& operator << (std::ostream& os, const z_normaliztion_conf_stats& nb)
{
    return os<<static_cast<const z_normaliztion_conf&>(nb)<<" column stats "<<nb.stats;
}

z_normaliztion_conf_stats::z_normaliztion_conf_stats(column_id id, double mean, double std) :
            z_normaliztion_conf{id}, stats{mean, std}
{
}

std::string to_string(const z_normaliztion_conf_stats& n)
{
    return to_string(static_cast<const z_normaliztion_conf&>(n));
}

std::string to_string(const max_normalization_stats& n)
{
    return to_string(static_cast<const max_normalization&>(n));
}

std::string to_string(const minmax_normalization_stats& n)
{
    return to_string(static_cast<const minmax_normalization&>(n));
}

max_normalization_stats::max_normalization_stats(column_id id, double minVal, double maxVal) :
            max_normalization{id}, values_range{minVal, maxVal}
{
    values_range = math::abs(values_range); // cannot call with this!!
}

std::ostream& operator << (std::ostream& os, const max_normalization_stats& nb)
{
        return os<<static_cast<const max_normalization&>(nb)<<" values range "<<nb.values_range;
}

std::ostream& operator << (std::ostream& os, const minmax_normalization_stats& nb)
{
        return os<<static_cast<const minmax_normalization&>(nb)<<", values range "<<nb.values_range;
}


minmax_normalization_stats::minmax_normalization_stats(column_id id, 
                    double a, double b, double min, double max) :
        minmax_normalization{id, a, b}, values_range{min, max}
{

}

