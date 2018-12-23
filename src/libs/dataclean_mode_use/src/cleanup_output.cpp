#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif  // SUPPORT_FOR_MODEL_USE
#include "libs/cleanup_operatiosn/cleanup_output.h"

cleanup_output make_output(normalized_result from, const std::string& )
{
    return cleanup_output{std::move(from.output)};
}

