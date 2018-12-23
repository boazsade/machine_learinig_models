#pragma once
#include "utils/math_datatype.h"
#include <boost/iterator/counting_iterator.hpp>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <vector>
#include <cmath>

namespace utils
{

double round_by(double from, unsigned int presition);

template<typename ResultType, typename It> inline
ResultType calc_standardstd(It from, It to, ResultType mean, std::size_t size)
{
    // this mean we don't have a variation, and in any case we would produce a NaN if we 
    // would try to run this (look at line 30)
   if (size <= 1) {    
       return 0;
   }
   // calculation is done by sigma 0 to n of absulute value of power from mean, then taking the squre root of the value
   ResultType ac = 
           std::accumulate(from, to, ResultType(), [mean](ResultType accum, auto d) {
                                                   accum += (d - mean) * (d - mean);
                                                   return accum;
                                               }
                                       );

    return std::sqrt(ac / (size - 1));
}

template<typename ResultType, typename It> inline
ResultType mean_value(It from, It to)
{
    if (from == to) {
        return ResultType{};
    } else {
        auto size = std::distance(from, to);
        auto sum =  std::accumulate(from, to, ResultType());
        return sum/size;
    }
}


template<typename ResultType, typename It> inline
math::statistical_data<ResultType> 
  mean_standard_dev(It from, It to, std::size_t size)
{
    auto mean = mean_value<ResultType>(from, to);
    return math::statistical_data<ResultType>{mean, calc_standardstd(from, to, mean, size)};
}

template<typename T> inline
T mean_distance(T val, int distance)
{
    using iterator_type = boost::counting_iterator<int>;
    T s = 1;
    T acc = std::accumulate(iterator_type(0), iterator_type(distance), s,
            [val](auto ac, auto) {
                ac *= val;
                return ac; 
            }
        );
    return acc;
}

template<typename T, typename It> inline
T moment(It from, It to, T mean, int order, 
    std::size_t size)
{
    T m = T();
    T accum = std::accumulate(from, to, m, [order, mean](T acc, T d) {
            T t = (d - mean);
            auto mult = mean_distance(t, order);
            acc += mult;
            return acc;
        }
    );
    m = (T)std::sqrt(accum / (size - 1));
    return m;
}

// for outliers detection
std::vector<double>
grubbstest(std::vector<double> input, double alpha);

template<typename It> inline
std::vector<double> grubbstest(It from, It to, double alpha) {
    return grubbstest({from, to}, alpha);
}


}   // end of namespace utils

