#pragma once
#include "libs/cleanup_operatiosn/column_task.h"
#include "libs/cleanup_operatiosn/cleanup_results.h"
#include <vector>

namespace details
{

using tasks_results = std::vector<column_task::result_type>;

cleanup_results make_results(tasks_results tr);
cleanup_results make_error_results();

}   //end of namespace details
