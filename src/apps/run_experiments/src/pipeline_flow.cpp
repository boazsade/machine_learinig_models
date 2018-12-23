#include "apps/run_experiments/pipeline_flow.h"
#include "apps/run_experiments/verifiers_task.h"
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/transform_action/mapvalues_task.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/system/exceptions.h"
#include "libs/classifier_models_actions/classifersmodels_task.h"
#include "libs/classifier_models_actions/classifer_model_interface.h"
#include "libs/model_data/training_input_task.h"
#include "libs/model_data/testing_input_task.h"
#include "logging.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

namespace pipeline
{

namespace
{

inline
read_step::read_action_t make_read_action(bool rand) {
    return rand ? read_step::read_action_t{rand_modeldata_read} : read_step::read_action_t{modeldata_read};
}

}   // end of local namespace 

read_step::read_step(const config& conf) :
    source_location{conf.source_location},
    iterations{conf.iterations},
    model_builders(conf.model_builders),
    models_experiments(conf.models_experiments),
    inputs(conf.inputs), read_action{make_read_action(conf.randomized)}
{
}


classifier::read_result read_step::read() const
{
    return preform_action(read_action, source_location);
}


train_data read_step::cast(const read_step& from)
{
    return train_data{from};
}

void read_step::stream(std::ostream& s) const
{
    s<<"read step: source location - "<<source_location<<", iterations: "<<iterations<<"\nmodels to train:\n";
    std::copy(std::begin(model_builders), std::end(model_builders), std::ostream_iterator<trainers::value_type>(s, ", "));
    s<<"\nexperiments to run\n";
    std::copy(std::begin(models_experiments), std::end(models_experiments), std::ostream_iterator<experiments::value_type>(s, ", "));
    s<<"\ndata processing policy "<<inputs.train_reader<<" and for testing "<<inputs.test_reader;
}


std::ostream& operator << (std::ostream& os, const read_step& pr)
{
    pr.stream(os);
    return os;
}

classifier::read_result read(const read_step& conf)
{
    return conf.read();
}

///////////////////////////////////////////////////////////////////////////////


train_data::train_data(const read_step& from) :
    iterations(from.iterations), model_builders(from.model_builders),
    models_experiments(from.models_experiments),
    inputs(from.inputs)
{
}

train_data::train_data(trainers&& tr,
            experiments&& eprs,
            source_policy&& sp) : 
                model_builders{std::move(tr)},
                models_experiments{std::move(eprs)},
                inputs{std::move(sp)}
{
}

void train_data::stream(std::ostream& s) const
{
    s<<"train data - iterations: "<<iterations<<"\nmodels for training\n";
    std::copy(std::begin(model_builders), std::end(model_builders), std::ostream_iterator<trainers::value_type>(s, ", "));
    s<<"\nexperiments to run\n";
    std::copy(std::begin(models_experiments), std::end(models_experiments), std::ostream_iterator<experiments::value_type>(s, ", "));
    s<<"\ndata processing policy "<<inputs.train_reader<<" and for testing "<<inputs.test_reader;

}

std::size_t train_data::expers_count() const
{
    return models_experiments.size();
}

std::size_t train_data::trainers_count() const
{
    return model_builders.size();
}

train_data::const_iterator train_data::begin() const
{
    return train_action_iterator{iterations, model_builders.begin(), 
        models_experiments, inputs.train_reader, inputs.test_reader
    };
}

train_data::const_iterator train_data::end() const
{
    return train_action_iterator{iterations, model_builders.end(), 
        models_experiments, inputs.train_reader, inputs.test_reader
    };
}

std::ostream& operator << (std::ostream& os, const train_data& td)
{
    td.stream(os);
    return os;
}

std::size_t tasks_count(const train_data& td)
{
    return td.expers_count() * td.trainers_count();
}

///////////////////////////////////////////////////////////////////////////////

train_step::train_step(const trainer& m2t,
            const experiments& expr,
            const training_reader& tr,
            const testing_reader& tstr,
            unsigned int iters) :
               iterations(iters),
               train_read{tr}, test_read{tstr},
               train_action{m2t},
               expers(expr)
{
}

std::string train_step::name() const
{
    std::ostringstream buff;
    buff<<train_action;
    return buff.str();
}

std::string name(const train_step& ts)
{
    return ts.name();
}

test_step train_step::run(const input_type& input,
        lookup::lookuptable_transform transformer) const
{
    LOG_INFO<<"running trainng step on "<<*this;
    auto sd { preform_action(train_read, input) };  // get data for the train
    actions::labels_data(sd) =  preform_action(transformer, actions::labels_data(sd));  // use lookup table to convert labels to classes
    auto ac {preform_action(train_action, // train the model
                actions::extend_train_input{sd, preform_action(transformer, lookup::uniqued_op)}
            )
    };     
    return  test_step{expers, test_read, std::move(ac), train_action.type() };  // make next move
}

void train_step::stream(std::ostream& os) const
{
    os<<"train step - iterations: "<<iterations<<", model: "<<train_action<<", source policy "<<train_read<<" and for testging "<<test_read;
}

std::ostream& operator << (std::ostream& os, const train_step& pt)
{
    pt.stream(os);
    return os;
}


test_step train(const train_step& trainer, 
        const train_step::input_type& input,
        lookup::lookuptable_transform transformer)
{
    return trainer.run(input, transformer);
}

///////////////////////////////////////////////////////////////////////////////

train_action_iterator::train_action_iterator(unsigned int iters, 
                    train_data::trainers::const_iterator train_it_start,
                    const train_data::experiments& exr,
                    const train_data::training_reader& tr,
                    const train_data::testing_reader& tstr) :
    iterations{iters}, current{train_it_start}, 
    expers{&exr}, train_read{&tr}, test_reader{&tstr}
{
}

bool train_action_iterator::valid() const
{
    return expers && train_read && test_reader;
}

bool train_action_iterator::equal(const train_action_iterator& other ) const
{
    return other.current == current;
}

void train_action_iterator::increment()
{
    if (valid()) {
        ++current;
    }
}

train_step train_action_iterator::dereference() const
{
    if (!valid()) {
        throw std::runtime_error{"trying to dereference invalid train action iterator"};
    }
    return train_step{*current, *expers, *train_read, *test_reader, iterations};
}

void train_action_iterator::decrement()
{
    --current;
}

void train_action_iterator::advance(difference_type n)
{
    current = std::next(current, n);
}


///////////////////////////////////////////////////////////////////////////////

test_step::test_step(const experiments& exprs,
            const testing_reader& tr,
            train_step::result_type act,
            actions::model_type mt) :
    action(std::move(act), mt), reader{tr}, expers(exprs)
{
}

std::string test_step::name() const
{
    std::ostringstream buff;
    buff<<action;
    return buff.str();
}

std::string name(const test_step& ts)
{
    return ts.name();
}

void test_step::stream(std::ostream& os) const
{
    os<<"test step - action "<<action<<", data source "<<reader<<", experiments ";
    std::copy(std::begin(expers), std::end(expers), 
            std::ostream_iterator<experiments::value_type>(os, ", "));
}

test_step::result_type test_step::run(const input_type& input,
                        lookup::lookuptable_transform transformer) const
{
    LOG_INFO<<"running test on "<<*this;
    auto data { preform_action(reader, input) };                     // get the data for the test
    auto td {preform_action(action, actions::samples_data(data))};   // run the test
    auto labels = preform_action(transformer, actions::labels_data(data)); // extract label and transform them
    return std::make_tuple(td, labels);                              // the data for the experiments
}

test_step::const_iterator test_step::begin() const
{
    return const_iterator{expers.begin(), action.type()};
}

test_step::const_iterator test_step::end() const
{
    return const_iterator{expers.end(), actions::UNKOWN_MODEL_TYPE};
}

std::ostream& operator << (std::ostream& os, const test_step& ts)
{
    ts.stream(os);
    return os;
}

test_step::result_type test(const test_step& pt, 
        const test_step::input_type& input,
        lookup::lookuptable_transform transformer)
{
    return pt.run(input, transformer);
}

///////////////////////////////////////////////////////////////////////////////


experiment_step::experiment_step(action_type act) :
            action{std::move(act)}
{
}

std::string experiment_step::name() const
{
    std::stringstream buff;
    buff<<action;
    return buff.str();
}

std::string name(const experiment_step& er) 
{
    return er.name();
}

experiment_step::result_type experiment_step::run(const arg_type& arg) const
{
    LOG_INFO<<"runing experiment on "<<*this;
    return preform_action(action, arg);
}


void experiment_step::stream(std::ostream& s) const
{
    s<<"experiment - "<<action;
}

std::ostream& operator << (std::ostream& os, const experiment_step& es)
{
    es.stream(os);
    return os;
}

experiment_step::result_type experiment(const experiment_step& es,
        const  experiment_step::arg_type& arg)
{
    return es.run(arg);
}


///////////////////////////////////////////////////////////////////////////////

experiments_iterator::experiments_iterator(internal_iter it, actions::model_type t) :
    current(it), action(t)
{
}

void experiments_iterator::decrement()
{
    --current;
}

void experiments_iterator::increment()
{
    ++current;
}

void experiments_iterator::advance(difference_type n)
{
    current = std::next(current, n);
}

bool experiments_iterator::equal(experiments_iterator const& other) const
{
    return other.current == current;
}

const experiment_step experiments_iterator::dereference() const
{
    return experiment_step{
        preform_action(action, *current)
    };//experiment_step{*current};
}


}   // end of pipeline
