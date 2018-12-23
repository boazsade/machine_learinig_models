#include "math_datatype.h"
#include <iostream>

namespace math
{

template<typename T> inline
std::ostream& operator << (std::ostream& os, const statistical_data<T>& sd)
{
    return os<<"mean: "<<mean(sd)<<", standard deviation: "<<standard_dev(sd);
}

template<typename T> inline
std::ostream& operator << (std::ostream& os, const min_max<T>& mm)
{
    return os<<"min: "<<minimum(mm)<<", max: "<<maximum(mm);
}

}   // end of namespace math

