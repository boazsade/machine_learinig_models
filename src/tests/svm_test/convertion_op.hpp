#pragma once
#include <boost/multi_array.hpp>

namespace 
{

template <typename T, std::size_t... vs> struct c_array;
template <typename T, std::size_t v, std::size_t... vs> struct c_array<T, v, vs...>
{ 
    typedef typename c_array<T, vs...>::type type[v]; 
};
template <typename T> struct c_array<T> 
{ 
    typedef T type; 
};
template <typename T, std::size_t level> struct nested_initializer_list
{
      typedef std::initializer_list<typename nested_initializer_list<T, level-1>::type> type;
};
template <typename T> struct nested_initializer_list<T, 1>
{
      typedef std::initializer_list<T> type;
};
template <typename T>
void fill_array(const T& l, typename c_array<T>::type& a)
{
      a = l;
}
template <typename T, int dim, int... dims>
void fill_array(typename nested_initializer_list<T, sizeof...(dims)+1>::type l,
                       typename c_array<T, dim, dims...>::type& a)
{
    assert(l.size() == dim); // could be a static assert in C++14
    int i=0;
    for (auto it = l.begin(); it != l.end(); ++it, ++i) {
        fill_array<T, dims...>(*it, a[i]);
    }
}
template <typename T, int... dims>
boost::multi_array<T, sizeof...(dims)>
make_multi_array(typename nested_initializer_list<T, sizeof...(dims)>::type l)
{
    typename c_array<T, dims...>::type a; // Multidimensional C array
    fill_array<T, dims...>(l, a);
    boost::const_multi_array_ref<T, sizeof...(dims)> mar(
                reinterpret_cast<const T*>(a),
                std::array<int, sizeof...(dims)>{dims...});
    return boost::multi_array<T, sizeof...(dims)>(mar);
}

}   // end of namespace

