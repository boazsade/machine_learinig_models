#pragma once
#include "utils/math_utils.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <vector>

namespace utils
{

namespace detail
{

template<typename T>
void zscore_transform(std::vector<T>& data)
{
    auto mean = mean_value<T>(std::begin(data), std::end(data));
    auto std = moment(std::begin(data), std::end(data), mean, 2, data.size());
    if (std == static_cast<T>(0)) {
        throw std::runtime_error{"all values for z-score for KS test are the same (" + std::to_string(data[0]) +"), cannot do preform z-score transform"};
    } else {
        if (std::isnan(std)) {
            throw std::range_error{"z-score in KS test failed - the value calculated for standard deviation is invalid (std: " + std::to_string(std) + ")"};
        } else {
            std::transform(std::begin(data), std::end(data), 
                           std::begin(data), [mean, std](auto x) {
                                        return (x - mean)/std;
                           }
            );
            std::sort(std::begin(data), std::end(data));
        }
    }
}

template<typename T> inline
T distances(const std::vector<T>& source,
            const std::vector<T>& target,
            const T size_source, const T size_target)
{
    std::size_t t = 0u, s = 0u;
    const std::size_t te = target.size();
    const std::size_t se = source.size();
    T d = T{}, fn_s = T{}, fn_t = T{};
    while (t < te && s < se) {
        auto d1 = source[s];
        auto d2 = target[t]; 
        if (d1 <= d2) {
            fn_s = s++/size_source;
        }
        if (d2 <= d1) {
            fn_t = t++ / size_target;
        }
        auto dt = std::fabs(fn_t - fn_s);
        if (dt > d) {
            d = dt;
        }
    }
    return d;
}

template<class T> inline
T probks(T alam, int iterations)
{
    static const T EPS1 = 0.0001;
    static const T  EPS2 = 1.0e-9;
    const T a2 = static_cast<T>(-2.0) * alam * alam;
    T fac = static_cast<T>(2.0), sum = static_cast<T>(0.0), termbf = static_cast<T>(0.0);

    for (auto j = 1; j <= iterations; j++) {
        auto term = fac * std::exp(a2 * j * j);
        sum += term;
        if ((std::fabs(term) <= (EPS1 * termbf)) || (std::fabs(term) <= (EPS2 * sum))) {
            return sum;
        }
        fac = -fac;
        termbf = std::fabs(term);
    }
    return 1.0;
}

template<typename T> inline
T prob_calc(T en, T d)
{
    static const T factor1 = 0.12;  // WTF?
    static const T factor2 = 0.11;  // WTF?
    static const int iterations = 100; // see above the ???

    auto t = factor2/en;
    en += factor1 + t;
    en *=  d;
    return  probks(en, iterations);
}

template<typename T, typename Fn> inline
T calcualte_distance(const std::vector<T>& data, 
        Fn func, const T en)
{
    T fo = 0.0;
    const std::size_t n = data.size();
    auto d = T{};
    for (size_t j=0;j<n;j++) {
        auto fn = (j+1)/en;
        auto ff = func(data[j]);
        auto dt = std::max(std::fabs(fo-ff), std::fabs(fn-ff));
        if (dt > d) {
            d = dt;
        }

        fo = fn;
    }
    return d;
}

}   // end of namespace detail
}   // end of namespace utils
