#pragma once

//#include "classifers_models.h"
#include "classifer_model_interface.h"
#include "actions_types.h"
#include <iosfwd>
namespace classifier
{
// this "implements" the call to generation of model. This allow to hide the actual model
// type that is used for building the model itself, and calling this function would generate 
// along with the input the action that would be used to test the model
SVM_train::result_type
    preform_action(const SVM_train& model, 
            const  actions::training_input&  input);

EM_train::result_type
    preform_action(const EM_train& model, 
            const actions::training_input& input);

normal_bayes_train::result_type
    preform_action(const normal_bayes_train& model, 
            const actions::training_input& input);

random_tree_train::result_type
    preform_action(const random_tree_train& model, 
            const actions::training_input& input);

k_nearest_train::result_type
    preform_action(const k_nearest_train& model, 
            const actions::training_input& input);

logistic_regration_train::result_type
    preform_action(const logistic_regration_train& model, 
            const actions::training_input& input);

boosting_train::result_type
    preform_action(const boosting_train& model, 
            const actions::training_input& input);

decision_tree_train::result_type
    preform_action(const decision_tree_train& model, 
            const actions::training_input& input);

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

logistic_regration_test::result_type
    preform_action(const logistic_regration_test& model, 
            const actions::testing_input& input);

boosting_test::result_type
    preform_action(const boosting_test& model, 
            const actions::testing_input& input);

decision_tree_test::result_type
    preform_action(const decision_tree_test& model, 
            const actions::testing_input& input);

std::ostream& operator << (std::ostream&, const SVM_train&);
std::ostream& operator << (std::ostream&, const EM_train&);
std::ostream& operator << (std::ostream&, const normal_bayes_train&);
std::ostream& operator << (std::ostream&, const random_tree_train&);
std::ostream& operator << (std::ostream&, const k_nearest_train&);
std::ostream& operator << (std::ostream&, const logistic_regration_train&);
std::ostream& operator << (std::ostream&, const boosting_train&);
std::ostream& operator << (std::ostream&, const decision_tree_train&);
std::ostream& operator << (std::ostream&, const SVM_test&);
std::ostream& operator << (std::ostream&, const EM_test&);
std::ostream& operator << (std::ostream&, const normal_bayes_test&);
std::ostream& operator << (std::ostream&, const random_tree_test&);
std::ostream& operator << (std::ostream&, const k_nearest_test&);
std::ostream& operator << (std::ostream&, const logistic_regration_test&);
std::ostream& operator << (std::ostream&, const boosting_test&);
std::ostream& operator << (std::ostream&, const decision_tree_test&);

}   // end of namespace classifier

