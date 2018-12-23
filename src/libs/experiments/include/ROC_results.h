#pragma once
#include "experiments_types.h"
#include <iosfwd>

namespace exprs
{

struct ROC_results
{
    value_type area = 0.f;
    value_type rmse = 0.f;
    value_type accuracy = 0.f;
    data_array sensitivity;
    data_array fall_out;

    ROC_results() = default;
};

std::ostream& operator << (std::ostream& os, const ROC_results& roc);
}   // end of namespace exprs

