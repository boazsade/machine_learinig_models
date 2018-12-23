#include "apps/run_experiments/pipeline_flow.h"
#include "apps/run_experiments/pipeline_execute.h"
#include "logging.h"
#include <boost/range/algorithm/transform.hpp>
#include <iostream>
#include <future>
#include <thread>
#include <tuple>
#include <iterator>
#include <algorithm>

namespace pipeline
{

namespace 
{


using test_output = 
    std::tuple<exper_result::experiment_name, experiment_step::result_type>;
using task_interm_result = std::future<test_output>;
using output_results = std::vector<exper_result>;
using task_result = std::future<output_results>;

task_result run_task(train_step ts, const classifier::read_result& data);
output_results model2experiment(train_step ts, 
        const classifier::read_result& data);
task_interm_result experiment_task(experiment_step exper,
                const  experiment_step::arg_type& arg);
test_output run_experiment(experiment_step exper, 
        const experiment_step::arg_type& arg);

task_result run_task(train_step ts, const classifier::read_result& data)
{
    return std::async(std::launch::async, [ts, &data]() {
            return model2experiment(std::move(ts), data);
        }
    );
}

test_output run_experiment(experiment_step exper, 
        const experiment_step::arg_type& arg)
{
    return std::make_tuple(name(exper), experiment(exper, arg));
}

output_results model2experiment(train_step ts, 
        const classifier::read_result& data)
{
    using task_type = task_interm_result;
    using tasks = std::vector<task_type>;
    auto m_name = name(ts);
    LOG_INFO<<"running model experiment for "<<m_name<<" with data input - ";
    for (const auto& col : data) {
        LOG_INFO<<col;
    }
    // for a single train/test entry we have to 
    // 1. train
    auto test_s = train(ts, data);
    // 2. test
    auto test_res = test(test_s, data);
    // 3. verify the model through experiment
    tasks task_list; 
    boost::transform(test_s, std::back_inserter(task_list), [&test_res](auto expr)
            { return experiment_task(std::move(expr), test_res); }
        );
    // and collect the results
    output_results result;
    std::transform(std::begin(task_list), std::end(task_list), 
            std::back_inserter(result), [&m_name](auto& task) {
                auto res = task.get();
                return output_results::value_type{m_name, std::get<0>(res), std::get<1>(res)};
            }
    );
    return result;
}

task_interm_result experiment_task(experiment_step exper,
                const  experiment_step::arg_type& arg)
{
    return std::async(std::launch::async, [exper, &arg]() {
                return run_experiment(std::move(exper), arg);
            }
        );
}

experiment::result_type run_experiments(train_data&& td,
        const classifier::read_result& data)
{
    using tasks_output = std::vector<task_result>;
    // we are running all tasks from here - the number
    // of these tasks is related to number of models we 
    // are running
    LOG_INFO<<"running task for "<<td.trainers_count()<<" trainers with "<<td.expers_count()
        <<" experiments on dataset of "<<classifier::rows(data)<<" X "<<classifier::columns(data);
    experiment::result_type result;
    if (classifier::empty(data)) {
        LOG_ERROR<<"no data found - cannot run!!";
        return result;
    }
    tasks_output to; 
    boost::transform(td, std::back_inserter(to),
                    [&data](auto ts) { return run_task(std::move(ts), data); }
                );   
    std::for_each(std::begin(to), std::end(to), [&result](auto& task) {
            auto ts = task.get();
            result.insert(result.end(), std::begin(ts), std::end(ts));
        }
    );
    return result;
}

}   // end of local namespace

///////////////////////////////////////////////////////////////////////////////

exper_result::exper_result(const model_name& ml, const experiment_name& en) :
    model(ml), experiment(en)
{
}

exper_result::exper_result(const model_name& ml, const experiment_name& en,
        data_type d) :
    model(ml), experiment(en), data(std::move(d))
{
}

std::ostream& operator << (std::ostream& os, const exper_result& er)
{
    return os<<"model - "<<er.model<<", experiment "<<er.experiment<<", details\n"<<er.data;
}

///////////////////////////////////////////////////////////////////////////////

experiment::result_type 
    experiment::run(config&& conf) const
{
    LOG_INFO<<"starting experiments with data from "<<pipeline::source_path(conf)<<", we have "<<models_count(conf)<<" models and "<<pipeline::iterations(conf)<<" iterations to run";
    auto rs = config::cast(conf);
    auto all_data = rs.read();    
    LOG_INFO<<"read "<<all_data.size()<<" columns and "<<(all_data.empty() ? 0u : all_data[0].data.size())<<" rows"; 
    if (all_data.empty()) {
        LOG_ERROR<<"failed to read anything from the source at "<<pipeline::source_path(conf);
        throw std::runtime_error{"failed to read from source at " + pipeline::source_path(conf).string()};
    } else {
        if (all_data[0].data.empty()) {
            LOG_ERROR<<"no data read for the row - file is either empty or invalid format!!";
            throw std::runtime_error{"failed to read data or file empty at " + pipeline::source_path(conf).string()};
        }
    }
    auto td = read_step::cast(rs);
    return run_experiments(std::move(td), all_data);
}

}   // end of namespace pipeline

