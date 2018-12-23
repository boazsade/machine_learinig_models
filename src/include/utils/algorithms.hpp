#pragma once
#include <algorithm>
#include <boost/range/size.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <cmath>

template<typename ForwardIt, typename T, typename Compare = std::less<T>> inline
ForwardIt binary_find(ForwardIt first, ForwardIt last, const T& value, Compare comp={})
{
    // Note: BOTH type T and the type after ForwardIt is dereferenced 
    // must be implicitly convertible to BOTH Type1 and Type2, used in Compare. 
    // This is stricter than lower_bound requirement (see above)
        
    first = std::lower_bound(first, last, value, comp);
    return first != last && !comp(value, *first) ? first : last;
}

template<typename Range> inline
std::ostream& out_range(std::ostream& os, const Range& rng, const char* delim = ", ")
{
    boost::copy(rng, std::ostream_iterator<typename Range::value_type>(os, delim));
    return os;
}

template<typename T> inline
bool round_cmp(T left, T right, T epsilon)
{
    static_assert(std::is_floating_point<T>::value, 
            "this function is for floating point types only");
    return std::abs(left - right) < epsilon;
}

namespace utils
{
    using boost::size;
}   // end of namespace utils

