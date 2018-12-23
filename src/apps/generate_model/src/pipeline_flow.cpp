#include "apps/generate_model/pipeline_flow.h"
//#include "libs/models_actions/transform_class_task.h"
#include "libs/system/exceptions.h"
#include "libs/classifier_models_actions/classifersmodels_task.h"
#include "libs/classifier_models_actions/classifer_model_interface.h"
#include "libs/model_data/training_input_task.h"
#include "libs/model_data/testing_input_task.h"
#include "libs/transform_action/mapvalues_task.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/lookuptable_transform.h"
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
read_step::read_action_t 
    make_read_action(bool rand) {
        return rand ? read_step::read_action_t{rand_modeldata_read} : read_step::read_action_t{modeldata_read};
}

}   // end of local namespace

read_step::read_step(const config& conf) :
    source_location{conf.source_location},
    iterations{conf.iterations},
    model_builder(conf.model_builder),
    models_experiments(conf.models_experiments),
    inputs(conf.inputs), output_path(conf.output_path),
    read_action{make_read_action(conf.randomized)}
{
}


classifier::read_result read_step::read() const
{
#if 0
    if (!boost::filesystem::exists(source_location)) {
        LOG_ERROR<<"invalid configuration - file not found "<<source_location;
        throw invalid_conf{"path not found: " + source_location.string()};
    }
    std::ifstream input{source_location.string()};
    if (!input) {
        LOG_ERROR<<"invlid configuration - cannot open "<<source_location<<" for reading";
        throw invalid_conf{"failed to open source file " + source_location.string()};
    }
    // read the schema -
    auto sh = read_schema(input);
    if (sh.empty()) {
        LOG_ERROR<<"failed to read schema information from "<<source_location;
        throw invalid_conf{"failed to read schema information from source at " + source_location.string()};
    } else {
        LOG_INFO<<"successfully read schema from "<<source_location<<" with "<<sh.size()<<" entries";
    }
    // now read the input
    return classifier::read(input, sh); 
#endif
    return preform_action(read_action, source_location);
}


train_data read_step::cast(const read_step& from)
{
    return train_data{from};
}

void read_step::stream(std::ostream& s) const
{
    s<<"read step: source location - "<<source_location<<", output path: "<<output_path<<" iterations: "<<iterations<<"\nmodels to train: "<<model_builder;
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
    iterations(from.iterations), model_builder(from.model_builder),
    models_experiments(from.models_experiments),
    inputs(from.inputs), output_path(from.output_path)
{
}

void train_data::stream(std::ostream& s) const
{
    s<<"train data - iterations: "<<iterations<<"\nmodels for training "<<model_builder;
    s<<"\nexperiments to run\n";
    std::copy(std::begin(models_experiments), std::end(models_experiments), std::ostream_iterator<experiments::value_type>(s, ", "));
    s<<"\ndata processing policy "<<inputs.train_reader<<" and for testing "<<inputs.test_reader;

}

std::size_t train_data::expers_count() const
{
    return models_experiments.size();
}

std::ostream& operator << (std::ostream& os, const train_data& td)
{
    td.stream(os);
    return os;
}

std::size_t tasks_count(const train_data& td)
{
    return td.expers_count();
}

///////////////////////////////////////////////////////////////////////////////

train_step::train_step(const train_data& from) : 
               iterations(from.iterations),
               train_read{from.inputs.train_reader}, test_read{from.inputs.test_reader},
               train_action{from.model_builder},
               expers(from.models_experiments), output_path(from.output_path)
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
                    const lookup::lookuptable_transform& trans) const
{
    LOG_INFO<<"running trainng step on "<<*this;
#if 0
    auto sd { preform_action(train_read, input) };
    auto ac {preform_action(train_action, sd)};
    return  test_step{expers, test_read, std::move(ac), train_action.type(), output_path };
#endif
    auto sd { preform_action(train_read, input) };  // get data for train
    actions::labels_data(sd) =  preform_action(trans, actions::labels_data(sd));  // use lookup table to convert labels to classes
    auto ac {preform_action(train_action, 
                actions::extend_train_input{sd,
                                    preform_action(trans, lookup::uniqued_op)
                                }
            )
    };     // train the model
    return test_step{expers, test_read, std::move(ac), train_action.type(), output_path };
}

void train_step::stream(std::ostream& os) const
{
    os<<"train step - iterations: "<<iterations<<", model: "<<train_action<<", source policy "<<train_read<<" and for testging "<<test_read<<", saving location "<<output_path;
}

std::ostream& operator << (std::ostream& os, const train_step& pt)
{
    pt.stream(os);
    return os;
}


test_step train(const train_step& trainer, 
        const train_step::input_type& input,
        const lookup::lookuptable_transform& trans)
{
    return trainer.run(input, trans);
}

///////////////////////////////////////////////////////////////////////////////
//return train_step{*current, *expers, *train_read, *test_reader, iterations};
///////////////////////////////////////////////////////////////////////////////

test_step::test_step(const experiments& exprs,
            const testing_reader& tr,
            train_step::result_type act,
            actions::model_type mt,
            const path_type& pt) :
    action(std::move(act), mt), 
    reader{tr}, expers(exprs), out_location(pt)
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
    os<<", saving the data to "<<out_location;
}

test_output test_step::run(const input_type& input,
        const lookup::lookuptable_transform& trans) const
{
    LOG_INFO<<"running test on "<<*this;
    auto data { preform_action(reader, input) };    // read the data
    auto td {preform_action(action, actions::samples_data(data))};   // run the test
    auto labels = preform_action(trans, actions::labels_data(data));    // convert label by lookup table
#if 0
    const classdata_transform ct{action.type()};
    auto data { preform_action(reader, input) };    // read the data
    auto td{preform_action(action, std::get<0>(data))}; // do the testing
    auto ad{preform_action(ct, std::get<1>(data))}; // convert input to same format as testing result
    std::cout<<"data from test step function that is transformed to be used for model eval\n";                
    std::copy(std::begin(ad), std::end(ad), std::ostream_iterator<float>(std::cout, ","));
    std::cout<<std::endl;
    std::cout<<"data from test step function that was returned from the test\n";
    std::copy(std::begin(td), std::end(td), std::ostream_iterator<float>(std::cout, ","));
    std::cout<<std::endl;
#endif
    return test_output{
        out_location, begin(), end(), 
        std::get<1>(td), 
        std::make_tuple(std::get<0>(td), labels)
    };
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

test_output test(const test_step& pt, 
        const test_step::input_type& input,
        const lookup::lookuptable_transform& trans)
{
    return pt.run(input, trans);
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

///////////////////////////////////////////////////////////////////////////////


save_step::save_step(const path_type& op, arg_type ac) :
    out_location(op), action(ac)
{
}

save_step::result_type save_step::run() const
{
    return preform_action(action, out_location);
}

save_step::result_type save(const save_step& s)
{
    return s.run();
}

///////////////////////////////////////////////////////////////////////////////

test_output::test_output(const path_type& pt,
            iterator first_exp, iterator last_exp,
            save_action act, data_type d) :
                    s_action(std::move(act)),
                    expers_range(first_exp, last_exp),
                    experiments_data(std::move(d)),
                    location(pt)
{
}

test_output::iterator test_output::begin() const
{
    return std::get<0>(expers_range);
}

test_output::iterator test_output::end() const
{
    return std::get<1>(expers_range);
}

const test_output::data_type& test_output::data() const
{
    return experiments_data;
}

test_output::save_action test_output::action() const
{
    return s_action;
}

const test_output::path_type& test_output::output_path() const
{
    return location;
}

std::ostream& test_output::stream(std::ostream& os) const
{
    os<<"save location: "<<output_path()<<", number of experiments to run: "<<std::distance(begin(), end())<<
        ", action to run "<<action()<<" data for experiments "<<std::get<0>(data()).size()<<" and "<<std::get<1>(data()).size();
    return os;
}

save_step save_cast(const test_output& from)
{
    return save_step(from.output_path(), from.action());
}

const test_output::data_type& expers_data_cast(const test_output& from)
{
    return from.data();
}

std::ostream& save_step::stream(std::ostream& os) const
{
    return os<<"saving to "<<out_location<<" action "<<action;
}

std::ostream& operator << (std::ostream& os, const save_step& to)
{
    return to.stream(os);
}

std::ostream& operator << (std::ostream& os, const test_output& to)
{
    return to.stream(os);
}

}   // end of pipeline
