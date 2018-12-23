#include "libs/cleanup_operatiosn/no_normalized_task.h"
#include "logging.h"

normalized_data::result_type preform_action(const _normalized_nothing& , normalized_data::arg_type arg)
{
    LOG_DEBUG<<"not appling normalization - no need";
    return default_case(arg);
}

