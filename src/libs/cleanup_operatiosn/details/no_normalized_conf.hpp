#include "libs/cleanup_operatiosn/no_normalized_conf.h"
#include <iostream>

std::string to_string(const _normalized_nothing& )
{
    return "no normalization";
}

std::ostream& operator << (std::ostream& os, _normalized_nothing)
{
    return os<<"apply no normalization";
}

std::ostream& operator << (std::ostream& os, const _t_normalization& )
{
    return os<<"t_norm ";
}

std::string to_string(const _t_normalization& )
{
    return "t_normalization";
}

