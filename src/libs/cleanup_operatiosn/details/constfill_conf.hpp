#include "libs/cleanup_operatiosn/constfill_conf.h"
#include <iostream>

std::string to_string(const const_value_fix& cvf) 
{
    return "const_fix " + std::to_string(cvf.value);
}

std::ostream& operator << (std::ostream& os, const const_value_fix& cvf) 
{
    return os<<to_string(cvf)<<" "<<static_cast<const fillinvalid_base&>(cvf);
}

std::string to_string(basic_contval_fix b)
{
    using namespace std::string_literals;

    return "const value fix "s + std::to_string(b.value); 
}

std::ostream& operator << (std::ostream& os, basic_contval_fix b) 
{
    return os<<to_string(b);
}

