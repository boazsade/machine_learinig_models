#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/cleanup_output.h"

cleanup_output make_output(normalized_result from, const std::string& id)
{
    return cleanup_output{std::move(from.output), id};
}

