#pragma once
#include "column_task.h"
#include "dataclean_conf.h"
#include "cleanup_results.h"

struct cleanup_task
{
    using arg_type = dataclean_conf;
    using result_type = cleanup_results; 

    constexpr cleanup_task() = default;

    result_type operator () (const arg_type& conf) const;
};

