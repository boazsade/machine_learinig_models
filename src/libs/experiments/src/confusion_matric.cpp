#include "libs/experiments/confusion_matric.h"
#include <iostream>

namespace exprs
{
namespace binclass
{
namespace cm
{
std::ostream& operator << (std::ostream& os, result r)
{
    switch (r) {
        case result::TRUE_NEG:
            return os<<"true negative";
        case result::FALSE_NEG:
            return os<<"false negative";
        case result::TRUE_POS:
            return os<<"true positive";
        case result::FALSE_POS:
            return os<<"false positive";
        case result::ERROR:
            return os<<"error";
    }
}

result calculate(value_type measure, value_type prediction)
{
    if (measure > 0.f) {
        return static_cast<int>(prediction) == static_cast<int>(measure) ? result::TRUE_POS : result::FALSE_NEG;
    } else {
        return static_cast<int>(prediction) == static_cast<int>(measure) ? result::TRUE_NEG : result::FALSE_POS;
    }
}

}   // end of namesapce cm
}   // end of namespace binclass
}   // end of namespace exprs

