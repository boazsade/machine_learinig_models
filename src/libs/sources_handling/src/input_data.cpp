#include "libs/sources_handling/input_data.h"
#include <iostream>

std::ostream& operator << (std::ostream& os, const input_data& id)
{
    return os <<"{value:"<< id.value << " @ " << id.row<<"}";
}

std::istream& operator >> (std::istream& is, input_data& id)
{
    return is >> id.value >> id.row;
}

