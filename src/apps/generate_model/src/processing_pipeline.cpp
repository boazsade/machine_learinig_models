#include "apps/generate_model/processing_pipeline.h"
#include "apps/generate_model/actions_pipeline.h"
#include "libs/sources_handling/row_data_input.h"
#include "libs/system/exceptions.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace pipeline
{
config::config(const path_type& src_p, source_policy sm, 
            trainer train, verifier_types expr,
            unsigned int iters, const path_type& o,
            lookup::map_values_conf t, bool rand) : 
    source_location{src_p}, iterations{iters}, model_builder{train},
    models_experiments{expr}, inputs{std::move(sm)}, output_path(o),
    map_config{std::move(t)}, randomized{rand}
{
}

config::config(const path_type& src_p,
            source_policy sm,
            trainer tr, experiments exp,
            unsigned int iters, const path_type& o,
            lookup::map_values_conf t, bool rand) : 
                source_location(src_p), //label_column(id),
                iterations(iters), model_builder(std::move(tr)),
                models_experiments(std::move(exp)),
                inputs(std::move(sm)), output_path{o},
                map_config{std::move(t)}, randomized{rand}
{
}

void config::stream(std::ostream& s) const
{
    s<<"config: path for sources: "<<source_location<<", save location: "<<output_path<<", number of iterations: "<<iterations<<" randomized for model? "<<std::boolalpha<<randomized<<
        "\nmodel to run: "<<model_builder;
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
    return !(c.source_location.empty() || c.models_experiments.empty());
}

std::size_t iterations(const config& c)
{
    return c.iterations;
}

std::size_t experiment2run(const config& c)
{
    return c.models_experiments.size();
}

config::path_type source_path(const config& c)
{
    return c.source_location;
}

std::size_t models_count(const config& )
{
    return 1u;
}


///////////////////////////////////////////////////////////////////////////////
}   // end of namespace pipeline
