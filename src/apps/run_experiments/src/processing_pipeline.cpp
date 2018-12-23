#include "apps/run_experiments/processing_pipeline.h"
#include "apps/run_experiments/pipeline_flow.h"
#include "apps/run_experiments/actions_pipeline.h"
#include "libs/sources_handling/row_data_input.h"
#include "libs/system/exceptions.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace pipeline
{
config::config(const path_type& src_p, source_policy sm, 
            trainers::value_type train, verifier_types expr,
            unsigned int iters, lookup::map_values_conf t, bool rand) : 
    source_location{src_p}, iterations{iters}, model_builders{train},
    models_experiments{expr}, inputs{std::move(sm)},  target_mapper{std::move(t)},
    randomized{rand}
{
}

read_step config::cast(const config& from)
{
    return read_step{from};
}

config::config(const path_type& src_p,
            source_policy sm,
            trainers tr, 
            verifier_types expr,
            unsigned int iters,
            lookup::map_values_conf t, bool rand) : 
                source_location(src_p), 
                iterations(iters), model_builders(std::move(tr)),
                models_experiments{expr},
                inputs(std::move(sm)),  target_mapper{std::move(t)},
                randomized{rand}
{
}

config::config(const path_type& src_p,
            source_policy sm,
            trainers tr, experiments exp,
            unsigned int iters, lookup::map_values_conf t, 
            bool rand) : 
                source_location(src_p), 
                iterations(iters), model_builders(std::move(tr)),
                models_experiments(std::move(exp)),
                inputs(std::move(sm)),  target_mapper{std::move(t)},
                randomized{rand}
{
}

config::config(
            const path_type& src_p,
            source_policy sm,
            trainers::value_type train,
            experiments exp, 
            unsigned int iters, lookup::map_values_conf t,
            bool rand) :
                source_location(src_p), 
                iterations(iters), model_builders{train},
                models_experiments(std::move(exp)),
                inputs(std::move(sm)), target_mapper{std::move(t)},
                randomized{rand}
{
}

void config::stream(std::ostream& s) const
{
    s<<"config: path for sources: "<<source_location<<", number of iterations: "<<iterations<<", randomized model data ? "<<std::boolalpha<<randomized<<
        "\nmodels to run: ";
    std::copy(std::begin(model_builders), std::end(model_builders), std::ostream_iterator<trainers::value_type>(s, ", "));
    s<<"\nexperiments to test_run ";
    std::copy(std::begin(models_experiments), std::end(models_experiments), 
            std::ostream_iterator<experiments::value_type>(s, ", "));
    s<<"\ntraining input processing - traininng: "<<inputs.train_reader<<", testing: "<<inputs.test_reader;
}

std::ostream& operator << (std::ostream& os, const config& pc)
{
    pc.stream(os);
    return os;
}

bool good(const config& c)
{
    return !(c.source_location.empty() || c.models_experiments.empty() || 
            c.model_builders.empty());
}

std::size_t iterations(const config& c)
{
    return c.iterations;
}

std::size_t models_count(const config& c)
{
    return c.model_builders.size();
}

std::size_t experiment2run(const config& c)
{
    return c.models_experiments.size();
}

config::path_type source_path(const config& c)
{
    return c.source_location;
}
///////////////////////////////////////////////////////////////////////////////
}   // end of namespace pipeline
