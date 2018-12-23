#pragma once
#include "ks_details.hpp"
#include "ks_output.h"
#include <vector>
#include <cmath>

namespace utils
{

template<class T> inline
    T normal_CDF(T x) // Phi(-�~H~^, x) aka N(x)
{   
    return std::erfc(-x / std::sqrt(2)) / 2;
}


template<typename T> inline
kalmogorov_smirnov_output<T>
    calcualte_prob(const T s_size, const T t_size, const T d)
{
    auto en = std::sqrt(s_size * t_size / (s_size + t_size));
    auto prob =  detail::prob_calc(en, d);
    return kalmogorov_smirnov_output<T>{d, prob};
}

template<typename T, typename Fn> inline
kalmogorov_smirnov_output<T>
    ks_data_validation(std::vector<T> &data, Fn func)
{
    detail::zscore_transform(data);
    auto en = static_cast<T>(data.size());
    auto d = detail::calcualte_distance(data, func, en);
    en = std::sqrt(en);
    auto prob = detail::prob_calc(en, d);
    return kalmogorov_smirnov_output<T>{d, prob};
}

template<class T>  inline
kalmogorov_smirnov_output<T> 
    ks_test_accuracy(std::vector<T>& data, std::vector<T>& target) 
{
    detail::zscore_transform(data);
    detail::zscore_transform(target);
    auto data_size  = static_cast<T>(data.size());
    auto target_size  = static_cast<T>(target.size());
    auto d = detail::distances(data, target, data_size, target_size);
    return calcualte_prob(data_size, target_size, d);
}

}   // end of namespace utils

