#pragma once

#include "apps/generate_model/processing_pipeline.h"
#include "apps/generate_model/pipeline_flow.h"
#include "apps/generate_model/params_output.h"
#include "apps/generate_model/execution_pipline.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/transform_action/map_values_op.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/mapvalues_task.h"
#include "logging.h"
#include <functional>
#include <tuple>
#include <algorithm>
#include <iostream>

namespace pipeline
{
using experiments_range = 
    std::tuple<test_step::const_iterator, test_step::const_iterator>;

using train_arg_f = std::function<test_step(const read_step::result_type& input)>;
using test_arg_f = std::function<test_output(const read_step::result_type&)>;

constexpr struct _do_read{} do_read = _do_read{};
constexpr struct _do_start{} do_start = _do_start{};
constexpr struct _prepare_it{} prepare_it = _prepare_it{};
constexpr struct _initial_model{} initial_model = _initial_model{};
constexpr struct _expers{} experiments = _expers{};
constexpr struct _prep_save{}  prepare_save = _prep_save{};
constexpr struct _do_save{} do_save = _do_save{};
constexpr struct _finish_data_process{}  finish_process = _finish_data_process{};

read_step operator | 
    (const config& conf, _do_start);

read_step::result_type operator | 
    (const read_step& rs, _do_read);

train_data operator | 
    (const read_step& rs, _prepare_it);

train_step operator | 
    (const train_data& td, _initial_model);

train_arg_f operator |
    (const train_step& ts, const lookup::lookuptable_transform& lt);

test_arg_f operator |
    (const test_step& ts, const lookup::lookuptable_transform& lt);

test_step operator | 
    (const train_arg_f& ts, const read_step::result_type& input);

experiments_range operator | 
    (const test_output& ts, _expers);

test_output operator | 
    (const test_arg_f& ts, const read_step::result_type& input);

params_output operator | 
    (const test_output& input, experiments_range exprs);

save_step operator | 
    (const test_output& ts, _prep_save);

save_step::result_type operator | 
    (const save_step& s, _do_save);

lookup::lookuptable_transform
    operator | (const config& conf, 
            const classifier::read_result& data);
}   // end of namespace pipeline

