#pragma once
#include "experiments_types.h"
#include <cstdint>
#include <iosfwd>

namespace exprs
{
namespace binclass
{
namespace cm
{

enum class result : std::uint16_t
{
    TRUE_POS  = 0u,
    TRUE_NEG,
    FALSE_POS,
    FALSE_NEG,
    ERROR
};
std::ostream& operator << (std::ostream& os, result r);

result calculate(value_type measure, value_type prediction);

}   // end of namesapce cm
}   // end of namespace binclass
}   // end of namespace exprs

