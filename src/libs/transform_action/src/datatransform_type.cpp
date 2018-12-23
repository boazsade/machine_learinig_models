#include "libs/transform_action/datatransform_types.h"
#include "utils/dc_lookup_table.h"
#include <iostream>
#include <sstream>

std::string to_string (const int& a)
{
    return std::to_string(a);
}

namespace lookup
{

std::string to_string(const _dont_map_op& )
{
    return "no mapping";
}

std::ostream& operator << (std::ostream& os, _dont_map_op)
{
    return os<<to_string(dont_map_op);
}

std::string to_string(const _dont_trasform_op&)
{
    return "\"none\"";
}

std::ostream& operator << (std::ostream& os, _dont_trasform_op)
{
    return os<<to_string(dont_trasform_op);
}

std::string to_string(const table& t)
{
    std::ostringstream p;
    p<<t;
    return p.str();
}

std::string to_string(const _uniqued_op&)
{
    return "unique values";
}

std::ostream& operator << (std::ostream& os, _uniqued_op uq)
{
    return os<<to_string(uq);
}

}   // end of namespace lookup

