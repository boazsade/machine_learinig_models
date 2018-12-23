#pragma once
#include "math_utils.h"

namespace utils
{
///////////////////////////////////////////////////////////////////////////////
// do normalization for the range
// norm single value with a given min/max that we took from a prev calcualted values
// and the range into which it should be fitted
double min_max_norm(double input, const math::min_max<double>& data_range,
        const math::min_max<double>& fit_into);

// normalized a given range of values
std::vector<double> min_max_normalization(std::vector<double> on, 
        double minVal, double maxVal);

std::vector<double> min_max_normalization(std::vector<double> on,
                        const math::min_max<double>& data_range,
                        const math::min_max<double>& fit_into);


// z norm for a given value at location
double z_norm(double input, const math::statistical_data<double>& mean_std);

// z norm for a range of values - all values would be nornalized
std::vector<double> z_normalization(std::vector<double> on);
std::vector<double> z_normalization(std::vector<double> on,
                        const math::statistical_data<double>& mean_std);

// max norm with only given value - we have the min and the max in the 
// range that this value was originated from allready computed
// note that you must pass minmax into math::abs to ensure that these 
// are absolute values
double max_norm(double input, const math::min_max<double>& minmax);
// max norm for a given range - all values would fit into 
// the max value that we have in the range
std::vector<double> max_normalization(std::vector<double> on);
std::vector<double> max_normalization(std::vector<double> on,
                    const math::min_max<double>& minmax);

}   // end of namespace utils

