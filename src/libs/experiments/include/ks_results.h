#pragma once
#include "experiments_types.h"
#include "utils/ks_output.h"
#include <iosfwd>

namespace exprs
{
namespace ks
{

using result_type = utils::kalmogorov_smirnov_output<value_type>;


}   // end of namespace ks
}   // end of namespace exprs

namespace utils
{
template<typename T>
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_output<T>& ks);

template<>
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_output<float>& ks);
template<>
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_output<double>& ks);
}   // end of namespace utils
