#pragma once

#include "actions_pipeline.h"
#include "libs/scores_evaluation/verifier_types.h"
#include "libs/model_data/testing_input_task.h"
#include "libs/model_data/training_input_task.h"
#include "libs/model_data/running_mode.h"
#include "libs/sources_handling/row_data_input.h"
#include "libs/transform_action/map_values_conf.h"
#include "functional/functional.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <iosfwd>

namespace pipeline
{


struct config
{

    struct source_policy
    {
        using training_reader = extract4training;
        using testing_reader = extract4testing;

        source_policy() = default;

        template<typename SourceMode>
        source_policy(SourceMode sm) :
                train_reader{train_run<SourceMode>{sm.split, sm.target_id}},
                test_reader{test_run<SourceMode>{sm.split, sm.target_id}}
        {
        }
        training_reader train_reader;
        testing_reader  test_reader;
    };

    using trainer = model_generator;
    using experiments = std::vector<verifier_types >;
    using training_reader = source_policy::training_reader;
    using testing_reader = source_policy::testing_reader;
    using path_type = boost::filesystem::path;
    using label_id = std::string;

    config(const path_type& src_p,
            source_policy sm, 
            trainer train,
            verifier_types expr,
            unsigned int iters,
            const path_type& o,
            lookup::map_values_conf t,
            bool rand);


    config(const path_type& src_p,
            source_policy sm,
            trainer tr,
            experiments exp, 
            unsigned int iters,
            const path_type& o,
            lookup::map_values_conf t,
            bool rand);

    void stream(std::ostream& s) const;

    template<typename Expr>
    void add_experiment(Expr e)
    {
        models_experiments.emplace_back(e);
    }

    path_type source_location;
    unsigned int iterations = 1;
    trainer  model_builder;
    experiments models_experiments;
    source_policy inputs;
    path_type output_path;
    lookup::map_values_conf map_config;
    bool randomized = false;
};

std::ostream& operator << (std::ostream& os, const config& pc);
bool good(const config& c);
std::size_t iterations(const config& c);
std::size_t models_count(const config& c);
std::size_t experiment2run(const config& c);
config::path_type source_path(const config& c);



}   // end of namespace pipeline
