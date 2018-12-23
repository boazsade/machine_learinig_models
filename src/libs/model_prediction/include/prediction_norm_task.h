#pragma once
#include "prediction_row.h"
#include "libs/cleanup_operatiosn/no_normalized_conf.h"
#include "libs/cleanup_operatiosn/normalized_conf.h"

namespace prediction
{

row::data_type
    preform_action(const minmax_normalization_stats& norm, const row& r);

row::data_type
    preform_action(const z_normaliztion_conf_stats& norm, const row& r);

row::data_type
    preform_action(const max_normalization_stats& norm, const row& r);

row::data_type
      preform_action(_normalized_nothing, const row&);

}   // end of namespace prediction

