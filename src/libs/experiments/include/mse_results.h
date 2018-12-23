#pragma once
#include "experiments_types.h"
#include <iostream>

namespace exprs
{

struct mse_results
{
    using result_type = value_type;

    constexpr mse_results() = default;

    constexpr explicit mse_results(result_type r) : value{r}
    {
    }

    result_type value = result_type{};
};

std::ostream& operator << (std::ostream& os, const mse_results& mse);

}   // end of namespace exprs

