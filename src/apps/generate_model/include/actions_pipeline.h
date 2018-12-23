#pragma once
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/model_datatypes/model_types.h"
#include "functional/functional.hpp"
#include "libs/sources_handling/row_data_input.h"
#include <boost/filesystem.hpp>
#include <tuple>
#include <iostream>

// useful data types

// first actions to to read the schema
// the second action is to read the actual data, using the above action
// then we need to have an action for dealing with the way we are expracting data 
// for the training, and the testing
// the next option in to run the model and test them
// note that it would generate a new action - the testing, so we need to define it first

// define the actual actions
// for the first two actions we need an IO type like
struct null_args {};

///////////////////////////////////////////////////////////////////////////////
// action datatypes
struct extract4training
{
    using action_type = poli_function<classifier::read_result, 
                                          actions::training_input>;
    using arg_type = action_type::arg_type;
    using result_type = action_type::result_type;

    template<typename Act>
    extract4training(Act a) :
        action{std::move(a)}
    {
    }

    result_type operator () (const arg_type& a) const;


    std::ostream& print(std::ostream& os) const;
private:
    action_type action;
};
std::ostream& operator << (std::ostream& os, const extract4training&);

///////////////////////////////////////////////////////////////////////////////

struct extract4testing 
{
    using action_type = poli_function<classifier::read_result, 
                                          actions::model_samples>; 
    using arg_type = action_type::arg_type;
    using result_type = action_type::result_type;

    template<typename Act>
    extract4testing(Act a) :
        action{std::move(a)}
    {
    }

    result_type operator () (const arg_type& a) const;

    std::ostream& print(std::ostream& os) const;
private:
    action_type action;

};
std::ostream& operator << (std::ostream& os, const extract4testing&);
///////////////////////////////////////////////////////////////////////////////

struct model_saver {
    using action_type = poli_function<boost::filesystem::path, bool>;
    using arg_type = action_type::arg_type;
    using result_type = action_type::result_type;

    template<typename Act>
    explicit model_saver(Act a) :
        action(std::move(a))
    {
    }

    result_type operator () (const arg_type& a) const;

    std::ostream& print(std::ostream& os) const;

private:
    action_type action;
};

std::ostream& operator << (std::ostream& os, const model_saver& ms);


///////////////////////////////////////////////////////////////////////////////

struct model_tester 
{
    using save_action = model_saver::action_type;
    using result_type = std::tuple<actions::testing_output,
                                    save_action>;

    using action_type = poli_function<actions::testing_input, 
                                        result_type>;
    using arg_type = action_type::arg_type;

    template<typename Act>
    model_tester(Act a, actions::model_type mt) :
        action{std::move(a)}, m_type(mt)
    {
    }

    result_type operator () (const arg_type& a) const;

    const action_type& the_action() const; // allow access to the internal action

    std::ostream& print(std::ostream& os) const;

    actions::model_type type() const {
        return m_type;
    }
private:
    action_type action;
    actions::model_type m_type = actions::UNKOWN_MODEL_TYPE;

};
std::ostream& operator << (std::ostream& os, const model_tester&);

///////////////////////////////////////////////////////////////////////////////

struct model_generator
{
    using action_type = poli_function<actions::extend_train_input, 
                                    model_tester::action_type>;
    using arg_type = action_type::arg_type;
    using result_type = action_type::result_type;

    template<typename Act>
    model_generator(Act a, actions::model_type mt) :
        action{std::move(a)}, m_type(mt)
    {
    }

    result_type operator () (const arg_type& a) const;

    std::ostream& print(std::ostream& os) const;

    actions::model_type type() const {
        return m_type;
    }
private:
    action_type action;
    actions::model_type m_type = actions::UNKOWN_MODEL_TYPE;
};
std::ostream& operator << (std::ostream& os, const model_generator&);


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

model_saver::result_type
    preform_action(const model_saver& with,
            const model_saver::arg_type& args);

