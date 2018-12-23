#include "libs/experiments/mse_results.h"
#include <iostream>

namespace exprs
{

std::ostream& operator << (std::ostream& os, const mse_results& mse)
{
    return os<<"error value: "<<std::fixed<<mse.value;
}

}   // end of namespace exprs
