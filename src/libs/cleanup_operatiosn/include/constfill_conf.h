#pragma once
#include "fillinvalid_baseconf.h"

struct basic_contval_fix 
{
    double value = 0;
    
    constexpr basic_contval_fix() = default;
    constexpr explicit basic_contval_fix(double v) :
            value{v}
    {
    }
};
std::string to_string(basic_contval_fix f);
std::ostream& operator << (std::ostream& os, basic_contval_fix b); 

struct const_value_fix : basic_contval_fix,
    fillinvalid_base
{
    using fillinvalid_base::fillinvalid_base;


    const_value_fix() = default;
    template<typename It>
    const_value_fix(column_id id, It from, It to, double v) :
        basic_contval_fix{v}, fillinvalid_base{id, from, to}
    {
    }
};
std::string to_string(const const_value_fix&); 
std::ostream& operator << (std::ostream& os, const const_value_fix& kf);


