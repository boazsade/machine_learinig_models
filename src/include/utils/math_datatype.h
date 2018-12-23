#pragma once
#include <vector>
#include <algorithm>
#include <cmath>

namespace math
{

template<typename T>
struct statistical_data
{
    using value_type = T;

    constexpr statistical_data() = default;
    constexpr statistical_data(value_type m, value_type s) :
        mean{m}, standard_dev{s}
    {
    }

    value_type mean = value_type{};
    value_type standard_dev = value_type{};
};

template<typename T> inline 
constexpr bool operator == (const statistical_data<T>& left, const statistical_data<T>& right) {
    return left.mean == right.mean &&
        left.standard_dev == right.standard_dev;
}
template<typename T> inline 
constexpr bool operator != (const statistical_data<T>& left, const statistical_data<T>& right) {
    return !(left == right);
}
template<typename T> inline 
constexpr bool operator < (const statistical_data<T>& left, const statistical_data<T>& right) {
    return left.mean < right.mean &&
        left.standard_dev < right.standard_dev;
}
template<typename T> inline 
constexpr bool operator > (const statistical_data<T>& left, const statistical_data<T>& right) {
    return !(left < right ||
        left == right);
}
template<typename T> inline 
constexpr bool operator >= (const statistical_data<T>& left, const statistical_data<T>& right) {
    return (left > right ||
        left == right);
}
template<typename T> inline 
constexpr bool operator <= (const statistical_data<T>& left, const statistical_data<T>& right) {
    return (left < right ||
        left == right);
}

template<typename T> inline
constexpr T mean(const statistical_data<T>& sd) {
    return sd.mean;
}

template<typename T> inline
constexpr T standard_dev(const statistical_data<T>& st) {
    return st.standard_dev;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
struct min_max
{
    using value_type = T;

    constexpr min_max() = default;
    constexpr min_max(T mn, T mx) :
        min_val{mn}, max_val{mx}
    {
    }

    value_type min_val = value_type{};
    value_type max_val = value_type{};
};

template<typename T> inline 
constexpr bool operator == (const min_max<T>& left, const min_max<T>& right) {
    return left.min_val == right.min_val &&
        left.max_val == right.max_val;
}
template<typename T> inline 
constexpr bool operator != (const min_max<T>& left, const min_max<T>& right) {
    return !(left == right);
}
template<typename T> inline 
constexpr bool operator < (const min_max<T>& left, const min_max<T>& right) {
    return left.min_val < right.min_val &&
        left.max_val < right.max_val;
}
template<typename T> inline 
constexpr bool operator > (const min_max<T>& left, const min_max<T>& right) {
    return !(left < right ||
        left == right);
}
template<typename T> inline 
constexpr bool operator >= (const min_max<T>& left, const min_max<T>& right) {
    return (left > right ||
        left == right);
}
template<typename T> inline 
constexpr bool operator <= (const min_max<T>& left, const min_max<T>& right) {
    return (left < right ||
        left == right);
}

template<typename T> inline
constexpr typename min_max<T>::value_type difference(const min_max<T>& at) {
    return at.max_val - at.min_val;
}

template <typename T> inline 
constexpr min_max<T> invalid() {
    return  min_max<T> {std::numeric_limits<T>::min(),
                                std::numeric_limits<T>::max()
                };
}

template<typename T> inline
constexpr statistical_data<T> invalid_stat() {
    return statistical_data<T> { std::numeric_limits<T>::max(),
                                 std::numeric_limits<T>::max()
                            };
}

namespace details
{

template<typename T>
struct extract_result_type {
    using value_t = typename T::value_type; // assume stl container
};

template<typename T>
struct extract_result_type<T*>
{
    using value_t = T;
};

template<typename T>
struct extract_result_type<const T*>
{
    using value_t = T;
};

template<typename T, std::size_t N>
struct extract_result_type<T[N]>
{
    using value_t = T;
};

template<typename T, std::size_t N>
struct extract_result_type<const T[N]>
{
    using value_t = T;
};

}   // end of namespace details

template<typename Range> inline 
constexpr min_max<typename details::extract_result_type<Range>::value_t> 
    min_max_values(const Range& range) {
    using value_t = typename details::extract_result_type<Range>::value_t;
    auto mm = std::minmax_element(std::begin(range), std::end(range));
    if (mm.first == mm.second) {
        return min_max<value_t>{std::numeric_limits<value_t>::min(), std::numeric_limits<value_t>::max()};
    } else {
        return min_max<value_t>{*mm.first, *mm.second};
    }
}

// convert to abs values for both min and max, then create a new 
// object with the values set according to their order as abs values
template<typename T> inline
min_max<T> abs(min_max<T> value) {
    auto mn = std::abs(value.min_val);
    auto mx = std::abs(value.max_val);
    if (mn > mx) {
        std::swap(mn, mx);
    }
    return min_max<T>{mn, mx};
}

template<typename T> inline
constexpr T minimum(const min_max<T>& mm) {
    return mm.min_val;
}

template<typename T> inline 
constexpr T maximum(const min_max<T>& mm) {
    return mm.max_val;
}

template<typename T> inline
constexpr bool isinf(const min_max<T>& what) {
    return std::isinf(maximum(what)) ||
        std::isinf(minimum(what));
}

template<typename T>  inline
constexpr bool isnan(const min_max<T>& what) {
    return std::isnan(maximum(what)) ||
        std::isnan(minimum(what));
}

template<typename T> inline
constexpr bool isinf(const statistical_data<T>& what) {
    return std::isinf(mean(what)) ||
        std::isinf(standard_dev(what));
}

template<typename T> inline
constexpr bool isnan(const statistical_data<T>& what) {
    return std::isnan(mean(what)) ||
        std::isnan(standard_dev(what));
}


template<typename T> inline
constexpr bool bad(const min_max<T>& what) {
    return what == invalid<T>() ||
        isinf(what) || isnan(what);
}

template<typename T> inline
constexpr bool bad(const statistical_data<T>& what) {
    return what == invalid_stat<T>() || isinf(what) || isnan(what);
}


template<typename T> inline
constexpr bool good(const min_max<T>& what) {
    return !(bad(what));
}

template<typename T> inline
constexpr bool good(const statistical_data<T>& what) {
    return !bad(what);
}


///////////////////////////////////////////////////////////////////////////////
//
using x_list_type = std::vector<double>;
using fx_list_type = std::vector<double>;

}   // end of namespace math
