#pragma once
#include "action_conf.h"
#include "cleanup_output.h"
#include <boost/filesystem.hpp>

struct column_task
{
    using arg_type = column_actions;
    using result_type = cleanup_output;

    constexpr column_task() = default;

    result_type operator () (const arg_type& task_args, 
            boost::filesystem::path base_dir) const;
};

