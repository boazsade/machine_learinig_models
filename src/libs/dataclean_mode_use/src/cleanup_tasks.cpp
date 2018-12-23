#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif  // SUPPORT_FOR_MODEL_USE
#include "libs/cleanup_operatiosn/cleanup_tasks.h"
#include "details/clean_output_prep.hpp"
#include "details/cleanup_tasks.hpp"
#include <algorithm>

namespace details
{

cleanup_results make_results(tasks_results)
{
    return true;
}

cleanup_results make_error_results()
{
    return false;
}

}   // end of namespace details

