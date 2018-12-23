#pragma once
#include "processing_pipeline.h"
#include "verifiers_action.h"
#include "actions_pipeline.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/sources_handling/modeldata_read_task.h"
#include <boost/iterator/iterator_facade.hpp>
#include <iosfwd>

namespace pipeline
{

struct train_data;

struct read_step 
{
    //using experiment_call = config::experiment_call;
    using trainer = config::trainer;
    using experiments = config::experiments;
    using training_reader = config::training_reader;
    using testing_reader = config::testing_reader;
    using path_type = config::path_type;
    using source_policy = config::source_policy;
    using result_type = classifier::read_result;
    using read_action_t = 
        poli_function<modeldata_read_base::arg_type, modeldata_read_base::result_type>;
    //using label_id = config::label_id;
    friend struct train_data;

    read_step(const config& conf);

    result_type read() const;

    static train_data cast(const read_step& from);

    void stream(std::ostream& s) const;

private:
    path_type source_location;
    //label_id  label_column;
    unsigned int iterations = 1;
    trainer  model_builder;
    experiments models_experiments;
    source_policy inputs;
    path_type output_path;
    read_action_t read_action;
};

std::ostream& operator << (std::ostream& os, const read_step& pr);
read_step::result_type read(const read_step& conf);

///////////////////////////////////////////////////////////////////////////////

struct train_step;

struct train_data
{
   // using experiment_call = read_step::experiment_call;
    using trainer = read_step::trainer;
    using experiments = read_step::experiments;
    using training_reader = read_step::training_reader;
    using testing_reader = read_step::testing_reader;
    using source_policy = read_step::source_policy;
    using path_type = config::path_type;
    //using label_id = read::label_idp

    train_data(const read_step& from);

    void stream(std::ostream& s) const;

    std::size_t expers_count() const;

    friend struct train_step;

private:
    //label_id  label_column;
    unsigned int iterations = 1;
    trainer  model_builder;
    experiments models_experiments;
    source_policy inputs;
    path_type output_path;
};

std::size_t tasks_count(const train_data& td);

std::ostream& operator << (std::ostream& os, const train_data& td);

///////////////////////////////////////////////////////////////////////////////


struct test_step;
// work on a single model for training and testing
struct train_step 
{
    //using experiment_call = train_data::experiment_call;
    using trainer = train_data::trainer;
    using experiments = train_data::experiments;
    using training_reader = train_data::training_reader;
    using testing_reader = train_data::testing_reader;
    using result_type = trainer::result_type;
    using input_type = training_reader::arg_type;
    using path_type = config::path_type;
    //using label_id = train_data::label_id;
    train_step(const train_data& from);

    test_step run(const input_type& input,
            const lookup::lookuptable_transform& ) const;
    void stream(std::ostream& os) const;

    std::string name() const;

private:
    unsigned int iterations = 1;
    training_reader train_read;
    testing_reader     test_read;
    trainer train_action;
    experiments expers;
    path_type output_path;
};
std::ostream& operator << (std::ostream& os, const train_step& pt);
std::string name(const train_step& ts);

test_step train(const train_step& trainer, 
        const train_step::input_type& input,
        const lookup::lookuptable_transform& tras);

///////////////////////////////////////////////////////////////////////////////
// allow to run operation of save the model
struct save_step {
    using path_type = config::path_type;
    using arg_type = model_saver::action_type;
    using result_type = bool;

    save_step(const path_type& op, arg_type ac);

    result_type run() const;
    std::ostream& stream(std::ostream& os) const;

private:
    path_type out_location;
    model_saver action;
};

save_step::result_type save(const save_step& s);
std::ostream& operator << (std::ostream& os, const save_step& to);
///////////////////////////////////////////////////////////////////////////////
// a given step is a single experiment - 
// that is we are only generting single entry here
struct experiment_step
{
    using action_type = verify_action::action_type::result_type;
    using result_type = action_type::result_type;
    using arg_type = actions::experiment_input;

    experiment_step(action_type at);

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

///////////////////////////////////////////////////////////////////////////////

struct experiments_iterator :
    boost::iterator_facade<experiments_iterator,
                            const experiment_step,
                            boost::random_access_traversal_tag,
                            experiment_step
                    >
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

///////////////////////////////////////////////////////////////////////////////

struct test_output;

struct test_step
{
    //using experiment_call = train_step::experiment_call;
    using experiments = train_step::experiments;
    using testing_reader = train_step::testing_reader;
    using action_type = train_step::result_type;
    using tester = model_tester;
    using result_type = experiment_step::arg_type;
    using experiments_list = std::vector<experiment_step>;
    using input_type = testing_reader::arg_type;
    using const_iterator = experiments_iterator;
    using path_type = config::path_type;
    using save_action = save_step::arg_type;

    test_step(const experiments& exprs,
            const testing_reader& tr,
            train_step::result_type act,
            actions::model_type mt, 
            const path_type& pt);

    void stream(std::ostream& os) const;

    test_output run(const input_type& input,
            const lookup::lookuptable_transform& trans) const;

    const_iterator begin() const;
    const_iterator end() const;
    std::string name() const;

    static save_step make_save(const test_step& from);

private:
    tester      action;
    testing_reader reader;
    experiments expers;
    path_type out_location;
};
std::ostream& operator << (std::ostream& os, const test_step& pr);
test_output test(const test_step& pt, 
        const test_step::input_type& input,
        const lookup::lookuptable_transform& trans);

std::string name(const test_step& ts);

///////////////////////////////////////////////////////////////////////////////

struct test_output
{
    using save_action = test_step::save_action;
    using iterator = test_step::const_iterator;
    using data_type = test_step::result_type;
    using path_type = test_step::path_type;

    test_output(const path_type& pt,
            iterator first_exp, iterator last_exp,
            save_action act, data_type d);

    iterator begin() const;
    iterator end() const;

    const data_type& data() const;

    save_action action() const;

    const path_type& output_path() const;
    std::ostream& stream(std::ostream& os) const;

private:
    save_action s_action;
    std::tuple<iterator, iterator> expers_range;
    data_type experiments_data;
    path_type location;
};

save_step save_cast(const test_output& from);
const test_output::data_type& expers_data_cast(const test_output& from);
std::ostream& operator << (std::ostream& os, const test_output& to);

}  // end of namespace pipeline

