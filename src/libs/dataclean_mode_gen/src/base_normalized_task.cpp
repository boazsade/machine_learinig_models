#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/base_normalized_task.h"
#include "libs/sources_handling/source_data.h"
#include "utils/normalizations.h"
#include "utils/math_utils.h"
#include "utils/math_datatype_io.h"
#include "logging.h"
#include <algorithm>

namespace 
{

inline normalized_data::result_type make_results(normalized_data::arg_type data,
                                        column_info stats)
{
    return normalized_data::result_type{std::make_tuple(std::move(data), std::move(stats))};
}
// calcualte min/max values and the mean/std from this data
inline column_info make_stats(const source_data::algo_input& from) 
{
    auto minmax = math::min_max_values(from);
    auto stat = utils::mean_standard_dev<source_data::algo_input::value_type>(std::begin(from), std::end(from), from.size());
    return column_info{minmax, stat};
}

inline normalized_data::result_type 
    default_case(const normalized_data::arg_type& arg)
{
    return make_results(arg, column_info{});
}

// in case we didn't do anything..
inline normalized_data::result_type error_case() {
    static const normalized_data::arg_type empty = normalized_data::arg_type{};
    return default_case(empty);
}

template<typename Func, typename Action> inline
normalized_data::result_type 
    normalized(normalized_data::arg_type arg, const Action& action,
            Func norm_action)
{
    if (empty(arg)) {
        return default_case(arg);
    }
    LOG_DEBUG<<"processing norm with "<<size(arg)<<" by "<<action;
    auto data = transform_slice(std::make_pair(arg.begin(), arg.end()));
    auto info = make_stats(data);
    LOG_INFO<<"stats for column "<<action.column<<" are stats: "<<info.stats<<" values range "<<info.range;
    if (good(info)) {
        data = norm_action(data, info);
        merge_slice(data, std::make_pair(arg.begin(), arg.end()));
        LOG_DEBUG<<"finish to run normalization for "<<action;
        return make_results(arg, info);
    } else {
        LOG_ERROR<<"cannot normalized using norm the values for column stats "<<info.stats<<" are invalid for column "<<action.column;
        return error_case();
    }
}

}   // end of local namespace 

normalized_data::result_type 
    preform_action(const z_normaliztion_conf& znorm, 
            normalized_data::arg_type arg)
{
    return normalized(arg, znorm, [](const auto& d, const auto&  inf) {
                    return utils::z_normalization(d, inf.stats);
                }
            );
}

normalized_data::result_type 
    preform_action(const _t_normalization&, 
        normalized_data::arg_type )
{
    LOG_WARNING<<"tnorm  not supported, not appling normalization!!";
    return error_case();
}

normalized_data::result_type 
    preform_action(const minmax_normalization& minmax_norm, 
            normalized_data::arg_type arg)
{
    return normalized(arg, minmax_norm, [&minmax_norm](const auto& d, const auto& inf) {
                    return utils::min_max_normalization(d, inf.range, 
                                    math::min_max<double>{minmax_norm.min, minmax_norm.max});
            }
        );
}

normalized_data::result_type 
    preform_action(const max_normalization& mnorm, normalized_data::arg_type arg)
{
    return normalized(arg, mnorm, [](const auto& d, const auto& inf) {
                        return utils::max_normalization(d, inf.range);
                    }
            );
}

normalized_data::result_type preform_action(const _normalized_nothing& , normalized_data::arg_type arg)
{
    LOG_DEBUG<<"not appling normalization - no need";
    return default_case(arg);
}

