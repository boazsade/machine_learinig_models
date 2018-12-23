#include "libs/model_prediction/prediction_norm_task.h"
#include "libs/system/exceptions.h"
#include "utils/normalizations.h"
#include "utils/math_utils.h"
#include "utils/math_datatype_io.h"
#include "logging.h"
#include <limits>

namespace prediction
{

row::data_type
    preform_action(const minmax_normalization_stats& norm,
            const row& r)
{
    auto old_val = r[norm.column];
    LOG_DEBUG<<"running min/max normaliztion on column "<<norm.column<<" for value "<<old_val;
  
    const auto& data_range = norm.values_range;
    const math::min_max<double> bounds{norm.min, norm.max};
    if (math::good(data_range)) {
        old_val = utils::min_max_norm(old_val, data_range, bounds);
    } else {
        LOG_ERROR<<"got invalid entries for the norm function "<<data_range;
        throw invalid_conf{"invalid configuration given to min/max norm"};
    }
    return old_val;
}

row::data_type
    preform_action(const z_normaliztion_conf_stats& norm,
            const row& r)
{
    auto old_val = r[norm.column];
    LOG_DEBUG<<"running z-norm normaliztion on column "<<norm.column<<" for value "<<old_val;
    const auto& arg = norm.stats; 
    if (math::good(arg)) {
        old_val = utils::z_norm(old_val, arg);
    } else {
        LOG_ERROR<<"got invalid entries for the norm function "<<arg;
        throw invalid_conf{"invalid configuration given to z norm"};
    }
    return old_val;
}

row::data_type
    preform_action(const max_normalization_stats& norm,
            const row& r)
{
    auto old_val = r[norm.column];
    const auto& data_range = norm.values_range;
    LOG_DEBUG<<"processing max norm  for old value "<<old_val<<" on column "<<norm.column;
    if (math::good(data_range)) {
        old_val = utils::max_norm(old_val, data_range);
    } else {
        throw invalid_conf{"invalid configuration given to max norm"};
    }
    return old_val;
}

row::data_type
      preform_action(_normalized_nothing, const row&)
{
    LOG_ERROR<<"trying to call invalid case for action normaliztion!!";
    throw std::runtime_error{"this is an invalid case to call - action that do not normalized, this is an internal problem"};
    return std::numeric_limits<row::data_type>::max();
}

}   // end of namespace prediction

