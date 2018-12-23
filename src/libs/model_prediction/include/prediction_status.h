#pragma once
#include <cstdint>
#include <iosfwd>
#include <string>

namespace prediction
{

enum class status : std::uint8_t
{
    OK = 0,
    END_OF_FILE = 1,
    REMOVED = 2,
    INVALID = 3,
    MODEL_ERROR = 4
};

std::ostream& operator << (std::ostream& os, status s);
std::string to_string(status s);

}   // end of namespace prediction

