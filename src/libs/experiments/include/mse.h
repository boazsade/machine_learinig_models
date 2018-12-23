#pragma once

#include "mse_results.h"
#include <vector>

namespace exprs
{

// this would only work if you are working
// in non-classifiers problems
mse_results mse(const std::vector<value_type>& predictions, 
	const std::vector<value_type>& measured);

}	// end of namespace exprs

