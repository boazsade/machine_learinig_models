#include "libs/experiments/ks_results.h"
#include <iostream>


namespace utils
{

template<>
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_output<float>& ks)
{
    return os<<"KS score: "<<std::fixed<<ks.score<<", pvalue: "<<std::fixed<<ks.P_value;
}

template<>
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_output<double>& ks)
{
    return os<<"KS score: "<<std::fixed<<ks.score<<", pvalue: "<<std::fixed<<ks.P_value;
}

}   // end of namespace utils
