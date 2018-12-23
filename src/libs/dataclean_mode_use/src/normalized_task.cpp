#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif  // SUPPORT_FOR_MODEL_USE
#include "libs/cleanup_operatiosn/normalized_task.h"
#include "utils/normalizations.h"
#include "utils/math_utils.h"
#include "utils/math_datatype_io.h"
#include "logging.h"
#include <algorithm>

namespace 
{
    inline normalized_data::result_type
        default_case( normalized_data::arg_type arg)
    {
        return normalized_data::result_type{std::move(arg)};
    }

    inline normalized_data::result_type
        error_case()
    {
        static const normalized_data::result_type err = normalized_data::result_type{};
        return err;
    }

}   // end of local namespace

normalized_data::result_type 
    preform_action(const z_normaliztion_conf_stats& znorm, 
            normalized_data::arg_type arg)
{
    if (empty(arg)) {
        return default_case(std::move(arg));
    }
    LOG_DEBUG<<"processing znorm with "<<size(arg)<<" by "<<znorm;
    const auto& targ = znorm.stats;
    if (math::good(targ)) {
        std::transform(begin(arg), end(arg), begin(arg), [&targ] (auto val) {
                 val.value = utils::z_norm(val.value, targ);
                 return val;
            }
        );
    } else {
        LOG_ERROR<<"cannot normalized using znorm the values for column stats "<<targ<<" are invalid";
        return error_case();
    }
    return default_case(arg);
}

normalized_data::result_type 
    preform_action(const minmax_normalization_stats& minmax_norm, 
            normalized_data::arg_type arg)
{
    if (empty(arg)) {
        return error_case();
    }
    LOG_DEBUG<<"processing min max with "<<size(arg)<<" by "<<minmax_norm;
    const auto& data_range = minmax_norm.values_range;
    const math::min_max<double> bounds{minmax_norm.min, minmax_norm.max};
    if (math::good(bounds)) {
        std::transform(begin(arg), end(arg), begin(arg), [&data_range, &bounds](auto val) {
            val.value = utils::min_max_norm(val.value, data_range, bounds);
                return val;
            }
        );
        return default_case(arg);
    } else {
        LOG_ERROR<<"cannot normalized using min max norm, the values for column stats "<<bounds<<" are invalid";
        return error_case();
    }
}

normalized_data::result_type 
   preform_action(const max_normalization_stats& mnorm, 
        normalized_data::arg_type arg)
{
    if (empty(arg)) {
        return error_case();
    }
    LOG_DEBUG<<"processing max norm with "<<size(arg)<<" by "<<mnorm;
    const auto& data_range = mnorm.values_range;
    if (math::good(data_range)) {
        std::transform(begin(arg), end(arg), begin(arg), [&data_range](auto val) {
                val.value = utils::max_norm(val.value, data_range);
                return val;
            }
        );
        return default_case(arg);
    } else {
        LOG_ERROR<<"cannot normalized using max norm, the values for column stats "<<data_range<<" are invalid";
        return error_case();
    }
}


normalized_data::result_type preform_action(const _normalized_nothing& , normalized_data::arg_type arg)
{
    LOG_DEBUG<<"not appling normalization - no need";
    return default_case(arg);
}

normalized_data::result_type preform_action(const _t_normalization& , normalized_data::arg_type arg)
{
    LOG_WARNING<<"not appling normalization - t norm not supported";
    return default_case(arg);
}

