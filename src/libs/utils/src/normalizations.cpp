#include "utils/normalizations.h"


namespace utils
{

namespace
{

const auto minmax_norm = [](double input, double norm, double diff, double minVal, double min) {
    return diff * (input - min)/norm + minVal;
};

const auto z_normlize = [] (double input, double mean, double std) {
    input -= mean;
    return input / std;
};

const auto max_normlize = [] (double input, double max) {
    return input / max;
};

}   // end of local namespace


double min_max_norm(double input, const math::min_max<double>& data_range,
        const math::min_max<double>& fit_into)
{
    return minmax_norm(input, math::difference(data_range),
                        math::difference(fit_into), 
                        math::minimum(fit_into),
                        math::minimum(data_range)
                    );
}

std::vector<double> min_max_normalization(std::vector<double> on,
                        const math::min_max<double>& data_range,
                        const math::min_max<double>& fit_into)
{
    std::transform(std::begin(on), std::end(on), std::begin(on), [&data_range, &fit_into](auto elem) {
                return min_max_norm(elem, data_range, fit_into);
            }
    );
    return on;                                                                
}

std::vector<double> 
min_max_normalization(std::vector<double> on, double minVal, double maxVal)
{
    auto min_max = math::min_max_values(on);
    if (!good(min_max)) {
        throw std::runtime_error{"cannnot do min max normalization - range invalid"};
    }
    return min_max_normalization(on, min_max, math::min_max<double>{minVal, maxVal});
}

double z_norm(double input, const math::statistical_data<double>& mean_std)
{
    return z_normlize(input, math::mean(mean_std), math::standard_dev(mean_std));
}

std::vector<double> z_normalization(std::vector<double> on)
{
    if (on.empty()) {
        throw std::runtime_error{"cannot do Z normalization - empty range"};
    }
    auto sum = std::accumulate(std::begin(on), std::end(on), 0.0);
    double mean = sum / on.size();
    auto stdev = moment<double>(std::begin(on), std::end(on), mean, 2, on.size());
    return z_normalization(on, math::statistical_data<double>{mean, stdev});
}

std::vector<double> z_normalization(std::vector<double> on,
                        const math::statistical_data<double>& mean_std)
{
    if (math::standard_dev(mean_std) == 0.0 || std::isinf(math::standard_dev(mean_std)) || 
            std::isnan(math::standard_dev(mean_std))) {
        throw std::runtime_error{"got invalid calculation for z norm, the standard deviation is zero"};
    }
    std::transform(std::begin(on), std::end(on), std::begin(on), [mean_std](auto val) {
                return z_norm(val, mean_std);
            }
    );
    return on;
}

std::vector<double> max_normalization(std::vector<double> on)
{
    auto min_max = 
        math::abs(math::min_max_values(on));
    if (math::minimum(min_max) <= 0.0) {
        throw std::runtime_error{"failed to run max normalization, value is invalid"};
    }
    return max_normalization(on, min_max);
}

std::vector<double> max_normalization(std::vector<double> on,
                    const math::min_max<double>& minmax)
{
    auto max = math::maximum(minmax);
    std::transform(std::begin(on), std::end(on), std::begin(on), [max] (auto val) {
                return max_normlize(val, max);
            }
    );
    return on;
}

double max_norm(double input, const math::min_max<double>& minmax)
{
    return max_normlize(input, math::maximum(minmax));
}

}   // end of namespace utils

