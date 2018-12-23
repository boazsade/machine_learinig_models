#pragma once
#include "actions_types.h"
#include "row_data_input.h"
#include "experiment_action.h"
#include <tuple>
#include <iostream>

// useful data types

// first actions to to read the schema
// the second action is to read the actual data, using the above action
using read_data_a = experiment_action<std::istream, classifier::read_result>;
// then we need to have an action for dealing with the way we are expracting data 
// for the training, and the testing
using train_extraction_a = experiment_action<read_data_a::result_type, actions::training_input>;
using testing_extraction_a = experiment_action<read_data_a, actions::testing_input>;
// the next option in to run the model and test them
// note that it would generate a new action - the testing, so we need to define it first
using test_action = experiment_action<actions::testing_input, actions::testing_output>;
using train_action = experiment_action<actions::training_input, test_action>;

// define the actual actions
// for the first two actions we need an IO type like
struct null_args {};

template<typename AT>
struct action_datatype 
{
    using action_type = AT;
    using arg_type = typename action_type::arg_type;
    using result_type = typename action_type::result_type;

    template<typename AI>
    action_datatype(AI action_impl) :
        action(std::move(action_impl))
    {
    }

    result_type operator () (const arg_type& args) const
    {
        return preform_action(action, args);
    }

    std::ostream& print(std::ostream& os) const
    {
        return os<<action;
    }

private:
    action_type action;
};

///////////////////////////////////////////////////////////////////////////////
// action datatypes
struct extract4training : action_datatype<train_extraction_a>
{
    using base_t = action_datatype<train_extraction_a>;

    using base_t::base_t;
};
std::ostream& operator << (std::ostream& os, const extract4training&);

struct extract4testing : action_datatype<testing_extraction_a>
{
    using base_t = action_datatype<testing_extraction_a>;

    using base_t::base_t;
};
std::ostream& operator << (std::ostream& os, const extract4testing&);

struct model_generator : action_datatype<train_action>
{
    using base_t = action_datatype<train_action>;

    using base_t::base_t;
};
std::ostream& operator << (std::ostream& os, const model_generator&);

struct model_tester : action_datatype<test_action>
{
    using base_t = action_datatype<test_action>;

    using base_t::base_t;
};
std::ostream& operator << (std::ostream& os, const model_tester&);

///////////////////////////////////////////////////////////////////////////////
// the operations that are called for the client code
// 1. extract data for training
extract4training::result_type
    preform_action(const extract4training& with, 
            const extract4training::arg_type& args);
// 2. generate model
model_generator::result_type
    preform_action(const model_generator& with,
            const model_generator::arg_type& args);
// 3. exterat data for testing
extract4testing::result_type 
    preform_action(const extract4testing& with,
            const extract4testing::arg_type& args);
// 4. test model
model_tester::result_type
    preform_action(const model_tester& with, 
            const model_tester::arg_type& args);


