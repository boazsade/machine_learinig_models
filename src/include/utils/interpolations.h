#pragma once
#include "math_datatype.h"

namespace utils
{
// find a correction value for the "on" input"
// for all the functions:
// fx_list is the result of f(x) at the given x_list "locations"
// and x_missing is the "location" of the x that we want to have f(x) for
// the functions all returns this f(x) where x = x_missing
double linear_interpolation(const math::fx_list_type& fx_list,
        const math::x_list_type& x_list,
        math::x_list_type::value_type x_missing);

double linear_interpolation(const math::fx_list_type& fx_list);

double K_neighborhood(const math::fx_list_type& fx_list,
        const math::x_list_type& x_list,
        math::x_list_type::value_type x_missing);
double K_neighborhood(const math::fx_list_type& fx_list);

}   // end of namespace utils

