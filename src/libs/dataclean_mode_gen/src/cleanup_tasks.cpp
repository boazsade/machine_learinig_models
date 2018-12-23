#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/cleanup_tasks.h"
#include "libs/cleanup_operatiosn/cleanup_output.h"
#include "details/clean_output_prep.hpp"
#include "details/cleanup_tasks.hpp"
#include <algorithm>

namespace details
{

cleanup_results make_results(tasks_results tr)
{
    cleanup_results output;
    std::transform(std::begin(tr), std::end(tr), 
               std::back_inserter(output), [](auto& result) {
                   return std::make_tuple(stats(result), name(result));
                }
            );
    return output;
}

cleanup_results make_error_results()
{
    return cleanup_results{};
}

}   // end of namespace details

