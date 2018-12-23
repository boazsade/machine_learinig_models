#pragma once

//#include "classifers_models.h"
#include "classifer_model_interface.h"
#include "libs/model_datatypes/model_datatypes.h"
#include <tuple>
#include <iosfwd>
namespace classifier
{
// this "implements" the call to generation of model. This allow to hide the actual model
// type that is used for building the model itself, and calling this function would generate 
// along with the input the action that would be used to test the model
SVM_train::result_type
    preform_action(const SVM_train& model, 
            const  actions::extend_train_input&  input);

EM_train::result_type
    preform_action(const EM_train& model, 
            const actions::extend_train_input& input);

normal_bayes_train::result_type
    preform_action(const normal_bayes_train& model, 
            const actions::extend_train_input& input);

random_tree_train::result_type
    preform_action(const random_tree_train& model, 
            const actions::extend_train_input& input);

k_nearest_train::result_type
    preform_action(const k_nearest_train& model, 
            const actions::extend_train_input& input);

logistic_regression_train::result_type
    preform_action(const logistic_regression_train& model, 
            const actions::extend_train_input& input);

boosting_train::result_type
    preform_action(const boosting_train& model, 
            const actions::extend_train_input& input);

decision_tree_train::result_type
    preform_action(const decision_tree_train& model, 
            const actions::extend_train_input& input);

///////////////////////////////////////////////////////////////////////////////
// overload for save of the model
SVM_train_sv::result_type
    preform_action(const SVM_train_sv& model, 
            const  actions::extend_train_input&  input);

EM_train_sv::result_type
    preform_action(const EM_train_sv& model, 
            const actions::extend_train_input& input);

normal_bayes_train_sv::result_type
    preform_action(const normal_bayes_train_sv& model, 
            const actions::extend_train_input& input);

random_tree_train_sv::result_type
    preform_action(const random_tree_train_sv& model, 
            const actions::extend_train_input& input);

k_nearest_train_sv::result_type
    preform_action(const k_nearest_train_sv& model, 
            const actions::extend_train_input& input);

logistic_regression_train_sv::result_type
    preform_action(const logistic_regression_train_sv& model, 
            const actions::extend_train_input& input);

boosting_train_sv::result_type
    preform_action(const boosting_train_sv& model, 
            const actions::extend_train_input& input);

decision_tree_train_sv::result_type
    preform_action(const decision_tree_train_sv& model, 
            const actions::extend_train_input& input);

///////////////////////////////////////////////////////////////////////////////
SVM_test::result_type
    preform_action(const SVM_test& model, 
            const actions::testing_input& input);

EM_test::result_type
    preform_action(const EM_test& model, 
            const actions::testing_input& input);

normal_bayes_test::result_type
    preform_action(const normal_bayes_test& model, 
            const actions::testing_input& input);

random_tree_test::result_type
    preform_action(const random_tree_test& model, 
            const actions::testing_input& input);

k_nearest_test::result_type
    preform_action(const k_nearest_test& model, 
            const actions::testing_input& input);

logistic_regression_test::result_type
    preform_action(const logistic_regression_test& model, 
            const actions::testing_input& input);

boosting_test::result_type
    preform_action(const boosting_test& model, 
            const actions::testing_input& input);

decision_tree_test::result_type
    preform_action(const decision_tree_test& model, 
            const actions::testing_input& input);

///////////////////////////////////////////////////////////////////////////////
SVM_test_sv::result_type
    preform_action(const SVM_test_sv& model, 
            const actions::testing_input& input);

EM_test_sv::result_type
    preform_action(const EM_test_sv& model, 
            const actions::testing_input& input);

normal_bayes_test_sv::result_type
    preform_action(const normal_bayes_test_sv& model, 
            const actions::testing_input& input);

random_tree_test_sv::result_type
    preform_action(const random_tree_test_sv& model, 
            const actions::testing_input& input);

k_nearest_test_sv::result_type
    preform_action(const k_nearest_test_sv& model, 
            const actions::testing_input& input);

logistic_regression_test_sv::result_type
    preform_action(const logistic_regression_test_sv& model, 
            const actions::testing_input& input);

boosting_test_sv::result_type
    preform_action(const boosting_test_sv& model, 
            const actions::testing_input& input);

decision_tree_test_sv::result_type
    preform_action(const decision_tree_test_sv& model, 
            const actions::testing_input& input);

///////////////////////////////////////////////////////////////////////////////

SVM_save::result_type
    preform_action(const SVM_save& model, 
            const SVM_save::input_type& input);

EM_save::result_type
    preform_action(const EM_save& model, 
            const EM_save::input_type& input);

normal_bayes_save::result_type
    preform_action(const normal_bayes_save& model, 
            const normal_bayes_save::input_type& input);

random_tree_save::result_type
    preform_action(const random_tree_save& model, 
            const random_tree_save::input_type& input);

k_nearest_save::result_type
    preform_action(const k_nearest_save& model, 
            const k_nearest_save::input_type& input);

logistic_regression_save::result_type
    preform_action(const logistic_regression_save& model, 
            const logistic_regression_save::input_type& input);

boosting_save::result_type
    preform_action(const boosting_save& model, 
            const boosting_save::input_type& input);

decision_tree_save::result_type
    preform_action(const decision_tree_save& model, 
            const decision_tree_save::input_type& input);

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream&, const SVM_train&);
std::ostream& operator << (std::ostream&, const EM_train&);
std::ostream& operator << (std::ostream&, const normal_bayes_train&);
std::ostream& operator << (std::ostream&, const random_tree_train&);
std::ostream& operator << (std::ostream&, const k_nearest_train&);
std::ostream& operator << (std::ostream&, const logistic_regression_train&);
std::ostream& operator << (std::ostream&, const boosting_train&);
std::ostream& operator << (std::ostream&, const decision_tree_train&);
std::ostream& operator << (std::ostream&, const SVM_test&);
std::ostream& operator << (std::ostream&, const EM_test&);
std::ostream& operator << (std::ostream&, const normal_bayes_test&);
std::ostream& operator << (std::ostream&, const random_tree_test&);
std::ostream& operator << (std::ostream&, const k_nearest_test&);
std::ostream& operator << (std::ostream&, const logistic_regression_test&);
std::ostream& operator << (std::ostream&, const boosting_test&);
std::ostream& operator << (std::ostream&, const decision_tree_test&);

std::ostream& operator << (std::ostream&, const SVM_train_sv&);
std::ostream& operator << (std::ostream&, const EM_train_sv&);
std::ostream& operator << (std::ostream&, const normal_bayes_train_sv&);
std::ostream& operator << (std::ostream&, const random_tree_train_sv&);
std::ostream& operator << (std::ostream&, const k_nearest_train_sv&);
std::ostream& operator << (std::ostream&, const logistic_regression_train_sv&);
std::ostream& operator << (std::ostream&, const boosting_train_sv&);
std::ostream& operator << (std::ostream&, const decision_tree_train_sv&);
std::ostream& operator << (std::ostream&, const SVM_test_sv&);
std::ostream& operator << (std::ostream&, const EM_test_sv&);
std::ostream& operator << (std::ostream&, const normal_bayes_test_sv&);
std::ostream& operator << (std::ostream&, const random_tree_test_sv&);
std::ostream& operator << (std::ostream&, const k_nearest_test_sv&);
std::ostream& operator << (std::ostream&, const logistic_regression_test_sv&);
std::ostream& operator << (std::ostream&, const boosting_test_sv&);
std::ostream& operator << (std::ostream&, const decision_tree_test_sv&);

std::ostream& operator << (std::ostream&, const SVM_save&);
std::ostream& operator << (std::ostream&, const EM_save&);
std::ostream& operator << (std::ostream&, const normal_bayes_save&);
std::ostream& operator << (std::ostream&, const random_tree_save&);
std::ostream& operator << (std::ostream&, const k_nearest_save&);
std::ostream& operator << (std::ostream&, const logistic_regression_save&);
std::ostream& operator << (std::ostream&, const boosting_save&);
std::ostream& operator << (std::ostream&, const decision_tree_save&);

}   // end of namespace classifier

