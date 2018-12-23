#pragma once
#include "utils/math_datatype.h"

namespace cubic
{


using coordinates_type = math::fx_list_type;
using values_type = math::x_list_type;
struct spline_data;

// this function would do calcualte the missing value at location "at"
// after we have spline data.
values_type::value_type 
    interpole(const spline_data& data, coordinates_type::value_type at);

// please note that the values that we passing to the constructor must be 
// without the missing values that we are calculating!!
struct spline_data
{
    spline_data(coordinates_type xcords, values_type xval);
    spline_data(spline_data&&) = default;
    spline_data& operator = (spline_data&&) = default;

private:
    friend values_type::value_type 
        interpole(const spline_data& data, coordinates_type::value_type at);


    coordinates_type x_coordinate;
    values_type      x_values;
    values_type      second_deriv_vals;
};  


}   // end of namespace iterpolate
