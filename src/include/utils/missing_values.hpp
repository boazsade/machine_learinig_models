#pragma once
#include <cmath>
#include <type_traits>
#include <limits>


template<typename T> inline constexpr 
T missing_value() {
    return std::numeric_limits<T>::max();
}

template<typename T> inline
constexpr bool missing_value(T val) {
    return val == missing_value<T>();
}

template<typename T> inline
constexpr bool invalid_value(T val) {
    static_assert(std::is_floating_point<T>::value, 
            "this function must be called with floating point type only!!");
    return std::isinf(val) || std::isnan(val);
}

