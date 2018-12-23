#pragma once
#include "libs/cleanup_operatiosn/column_task.h"
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "libs/cleanup_operatiosn/normalized_task.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "libs/system/exceptions.h"
#include "logging.h"
//#include "libs/cleanup_operatiosn/map_values_op.h"
//#include "libs/cleanup_operatiosn/mapvalues_task.h"
//#include "libs/cleanup_operatiosn/mapvalues_default_task.h"
#include "logging.h"
#include <fstream>

namespace 
{
    column_task::result_type process_tasks(drop_unwanted::arg_type&& input, 
                                const column_task::arg_type& actions, 
                                const std::string& id)
    {
        auto data = preform_action(actions.drop(), input);
        data = preform_action(actions.fixes(), data);
        try {
            auto nd = preform_action(actions.normalization(), data); 
            return make_output(nd, id);
        } catch (const std::exception& e) {
            LOG_ERROR<<"failed to run normaliztion on column "<<id<<", error: "<<e.what();
            throw norm_error{id};
        }
    }

    column_task::result_type process(const column_task::arg_type& args, 
                            const boost::filesystem::path& input_file)
    {
        static const column_task::result_type err = column_task::result_type{};

        std::ifstream input{input_file.string()};
        if (!input) {
            LOG_ERROR<<"failed to read input from '"<<input_file<<"'";
            return err;
        } else {
            LOG_DEBUG<<"starting to process input from '"<<input_file<<"'";
            drop_unwanted::arg_type result{read(input, id(args))};
            if (empty(result)) {
                LOG_ERROR<<"failed to read any input from '"<<input_file<<"', cannot process";
                return make_output(normalized_result{}, id(args));
            } else {
                return process_tasks(std::move(result), args, id(args));
            }
        }
    }
}   // end of local namespace

column_task::result_type column_task::operator() (const arg_type& task_args,
                                    boost::filesystem::path base_dir) const
{
    base_dir /= data_location(task_args);
    if (boost::filesystem::exists(base_dir)) {
        return process(task_args, base_dir);
    } else {
        static const result_type err = result_type{};
        LOG_ERROR<<"failed to locate input for "<<id(task_args)<<" at "<<base_dir;
        return err;
    }
}

