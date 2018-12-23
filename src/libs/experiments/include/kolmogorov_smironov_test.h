#pragma once
#include "ks_results.h"
#include "experiments_types.h"
#include "utils/kalmogorov_smirnov.hpp"

namespace exprs
{
namespace ks
{

// note - we are passing by value since we are changing internally the data!!
result_type accuracy_test(data_array test_data, data_array measured_data);

}   // end of namespace ks

}   // end of namespace exprs

