#pragma once
#include "base_normalized_conf.h"
#include "utils/math_datatype.h"
#include <iosfwd>

struct minmax_normalization_stats : minmax_normalization
{
    using minmax_normalization::minmax_normalization;

    minmax_normalization_stats() = default;
    minmax_normalization_stats(column_id id, double mn, double mx,
                        double mnr, double mxr);

    math::min_max<double> values_range;    // the min and max in the range of value for this column
    
};
std::ostream& operator << (std::ostream& os, const minmax_normalization_stats& nb);
std::string to_string(const minmax_normalization_stats& n);

struct z_normaliztion_conf_stats : z_normaliztion_conf
{
    using z_normaliztion_conf::z_normaliztion_conf;

    z_normaliztion_conf_stats() = default;
    z_normaliztion_conf_stats(column_id id, double mean, double std);

    math::statistical_data<double> stats;
};
std::ostream& operator << (std::ostream& os, const z_normaliztion_conf_stats& nb);
std::string to_string(const z_normaliztion_conf_stats& n);

struct max_normalization_stats : max_normalization
{
    using max_normalization::max_normalization;

    max_normalization_stats() = default;

    max_normalization_stats(column_id id, double minVal, double maxVal);

    math::min_max<double> values_range;    // the min and max in the range of value for this column;
};
std::ostream& operator << (std::ostream& os, const max_normalization_stats& nb);
std::string to_string(const max_normalization_stats& n);

