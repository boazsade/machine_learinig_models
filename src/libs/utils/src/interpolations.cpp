#include "utils/interpolations.h"
#include "math_helper_func.hpp"

namespace utils
{

namespace
{

inline 
const math::x_list_type& default_x_vals()
{
    static const math::x_list_type values = math::x_list_type{1.0, 3.0};
    return values;
}

inline 
const math::x_list_type& default_x_vals_cubic()
{
    static const math::x_list_type values = math::x_list_type{1.0, 2.0, 4.0, 5.0};
    return values;
}

inline constexpr math::x_list_type::value_type 
default_x_missing()
{
    return 2.0;
}

inline constexpr math::x_list_type::value_type 
default_x_cubic_missing()
{
    return 3.0;
}

}   // end of local namespace


double linear_interpolation(const math::fx_list_type& fx_list)
{
    return linear_interpolation(fx_list, default_x_vals(), default_x_missing());
}

double linear_interpolation(const math::fx_list_type& fx_list,
        const math::x_list_type& x_list,
        math::x_list_type::value_type x_missing)
{
    return linear_interpulation_1d(x_list, fx_list, x_missing);
}

double K_neighborhood(const math::fx_list_type& fx_list)
{
    return K_neighborhood(fx_list, default_x_vals(), default_x_missing());
}

double K_neighborhood(const math::fx_list_type& fx_list,
        const math::x_list_type& x_list,
        math::x_list_type::value_type x_missing)
{
    return nearset_interpolations(x_list, fx_list, x_missing);
}

}   // end of namespace utils

