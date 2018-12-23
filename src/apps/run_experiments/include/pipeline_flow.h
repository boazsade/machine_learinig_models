#pragma once
#include "processing_pipeline.h"
#include "actions_pipeline.h"
#include "verifiers_action.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/sources_handling/modeldata_read_task.h"
#include <boost/iterator/iterator_facade.hpp>
#include <iosfwd>

namespace pipeline
{

struct train_data;

struct read_step 
{
    using trainers = config::trainers;
    using experiments = config::experiments;
    using training_reader = config::training_reader;
    using testing_reader = config::testing_reader;
    using path_type = config::path_type;
    using source_policy = config::source_policy;
    using read_action_t = 
        poli_function<modeldata_read_base::arg_type, modeldata_read_base::result_type>;
    friend struct train_data;

    read_step(const config& conf);

    classifier::read_result read() const;

    static train_data cast(const read_step& from);

    void stream(std::ostream& s) const;

private:
    path_type source_location;
    unsigned int iterations = 1;
    trainers  model_builders;
    experiments models_experiments;
    source_policy inputs;
    actions::model_type type = actions::UNKOWN_MODEL_TYPE;
    read_action_t read_action;
};

std::ostream& operator << (std::ostream& os, const read_step& pr);
classifier::read_result read(const read_step& conf);

struct train_step;
struct train_action_iterator;

struct train_data
{
    using trainers = read_step::trainers;
    using experiments = read_step::experiments;
    using training_reader = read_step::training_reader;
    using testing_reader = read_step::testing_reader;
    using source_policy = read_step::source_policy;

    using iterator = train_action_iterator;
    using const_iterator = train_action_iterator;

    train_data(const read_step& from);
    train_data(trainers&& tr,
            experiments&& eprs,
            source_policy&& sp);

    void stream(std::ostream& s) const;

    const_iterator begin() const;
    const_iterator end() const;

    std::size_t trainers_count() const;

    std::size_t expers_count() const;
private:
    unsigned int iterations = 1;
    trainers  model_builders;
    experiments models_experiments;
    source_policy inputs;
};

std::size_t tasks_count(const train_data& td);

std::ostream& operator << (std::ostream& os, const train_data& td);


struct test_step;
// work on a single model for training and testing
struct train_step 
{
    //using experiment_call = train_data::experiment_call;
    using trainer = train_data::trainers::value_type;
    using experiments = train_data::experiments;
    using training_reader = train_data::training_reader;
    using testing_reader = train_data::testing_reader;
    using result_type = trainer::result_type;
    using input_type = training_reader::arg_type;
    //using label_id = train_data::label_id;
    train_step(const trainer& m2t,
            const experiments& expr,
            const training_reader& tr,
            const testing_reader& tstr,
            unsigned int iters);

    test_step run(const input_type& input,
            lookup::lookuptable_transform transformer) const;
    void stream(std::ostream& os) const;

    std::string name() const;

private:
    unsigned int iterations = 1;
    training_reader train_read;
    testing_reader     test_read;
    trainer train_action;
    experiments expers;
};
std::ostream& operator << (std::ostream& os, const train_step& pt);
std::string name(const train_step& ts);

test_step train(const train_step& trainer, 
        const train_step::input_type& input,
        lookup::lookuptable_transform transformer);

struct train_action_iterator :
    boost::iterator_facade<train_action_iterator,
                    const train_step,
                    boost::random_access_traversal_tag,
                    train_step>
{

    train_action_iterator(unsigned int iters, 
                    train_data::trainers::const_iterator train_it_start,
                    const train_data::experiments& exr,
                    const train_data::training_reader& tr,
                    const train_data::testing_reader& tstr);
private:
    friend class boost::iterator_core_access;

    void decrement();
    void increment();
    void advance(difference_type n);
    bool equal(train_action_iterator const& other) const;
    train_step dereference() const;

    bool valid() const;

private:
    //const train_data::label_id* label_ld = nullptr;
    unsigned int iterations = 1; 
    train_data::trainers::const_iterator current;
    const train_data::experiments* expers = nullptr;
    const train_data::training_reader* train_read = nullptr;
    const train_data::testing_reader* test_reader = nullptr;
};


struct experiment_step;
struct experiments_iterator;
struct test_step
{
    //using experiment_call = train_step::experiment_call;
    using experiments = train_step::experiments;
    using testing_reader = train_step::testing_reader;
    using action_type = train_step::result_type;
    using result_type = actions::experiment_input;
    using tester = model_tester;
        /*std::tuple<action_type::result_type,    // result from the model test
                                   mlmodels::classes_data>;     // the actual data
                                   */
    using experiments_list = std::vector<experiment_step>;
    using input_type = testing_reader::arg_type;
    using const_iterator = experiments_iterator;

    test_step(const experiments& exprs,
            const testing_reader& tr,
            train_step::result_type act,
            actions::model_type mt);

    void stream(std::ostream& os) const;

    result_type run(const input_type& input,
        lookup::lookuptable_transform transformer) const;

    const_iterator begin() const;
    const_iterator end() const;
    std::string name() const;

private:
    tester      action;
    testing_reader reader;
    experiments expers;
};
std::ostream& operator << (std::ostream& os, const test_step& pr);
test_step::result_type test(const test_step& pt, 
        const test_step::input_type& input,
        lookup::lookuptable_transform transformer);

std::string name(const test_step& ts);

// a given step is a single experiment - 
// that is we are only generting single entry here
struct experiment_step
{
    using action_type = verify_action::action_type::result_type;//test_step::experiment_call;
    using result_type = action_type::result_type;
    using arg_type = test_step::result_type;

    experiment_step(action_type act);

    result_type run(const arg_type& arg) const;

    void stream(std::ostream& s) const;
    std::string name() const;

private:
    action_type action;
};
std::ostream& operator << (std::ostream& os, const experiment_step& es);
std::string name(const experiment_step& es);

experiment_step::result_type experiment(const experiment_step& es,
        const  experiment_step::arg_type& arg);

struct experiments_iterator :
    boost::iterator_facade<experiments_iterator,
    const experiment_step,
    boost::random_access_traversal_tag,
    experiment_step>
{
    using action_type = experiment_step::action_type;
    using internal_iter = config::experiments::const_iterator;

    experiments_iterator(internal_iter it, actions::model_type);
private:
    friend class boost::iterator_core_access;

    void decrement();
    void increment();
    void advance(difference_type n);
    bool equal(experiments_iterator const& other) const;
    const experiment_step dereference() const;

private:
    internal_iter current;
    verify_action action;
};
}  // end of namespace pipeline

