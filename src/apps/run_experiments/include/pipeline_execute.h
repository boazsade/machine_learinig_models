#pragma once
#include "actions_types.h"
#include <vector>
#include <string>
#include <iosfwd>

namespace pipeline
{

struct config;

struct exper_result
{
    using model_name = std::string;
    using experiment_name = std::string;
    using data_type = actions::experiment_output;

    exper_result() = default;
    exper_result(const model_name& ml,
            const experiment_name& en);
    exper_result(const model_name& ml,
           const experiment_name& en,
           data_type res); 

    model_name model;
    experiment_name experiment;
    data_type data;
};

std::ostream& operator << (std::ostream& os, const exper_result& er);

struct experiment
{
    using result_type = std::vector<exper_result>;

    result_type run(config&& conf) const;
};

}   // end of namespace pipeline

