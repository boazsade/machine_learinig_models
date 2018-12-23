#pragma once

//#include "classifers_models.h"
#include "classifer_predict_interface.h"
#include "libs/model_datatypes/model_datatypes.h"
#include <tuple>
#include <iosfwd>

namespace classifier
{
// run  prediction on the data - this would generate a result that the model is prediction
// for a given row in the input. Note that this interface assumes a list of rows and not a single
// entry, that is - we are not predicting for a single entry
SVM_predict::result_type
    preform_action(const SVM_predict& model, 
            const  SVM_predict::input_type&  input);

EM_predict::result_type
    preform_action(const EM_predict& model, 
            const EM_predict::input_type& input);

normal_bayes_predict::result_type
    preform_action(const normal_bayes_predict& model, 
            const SVM_predict::input_type& input);

random_tree_predict::result_type
    preform_action(const random_tree_predict& model, 
            const random_tree_predict::input_type& input);

k_nearest_predict::result_type
    preform_action(const k_nearest_predict& model, 
            const k_nearest_predict::input_type& input);

logistic_regression_predict::result_type
    preform_action(const logistic_regression_predict& model, 
            const SVM_predict::input_type& input);

boosting_predict::result_type
    preform_action(const boosting_predict& model, 
            const boosting_predict::input_type& input);

decision_tree_predict::result_type
    preform_action(const decision_tree_predict& model, 
            const decision_tree_predict::input_type& input);

// This would load a model from a saved location
SVM_load::result_type
    preform_action(const SVM_load& model, 
            const  SVM_load::input_type&  input);

EM_load::result_type
    preform_action(const EM_load& model, 
            const EM_load::input_type& input);

normal_bayes_load::result_type
    preform_action(const normal_bayes_load& model, 
            const SVM_load::input_type& input);

random_tree_load::result_type
    preform_action(const random_tree_load& model, 
            const random_tree_load::input_type& input);

k_nearest_load::result_type
    preform_action(const k_nearest_load& model, 
            const k_nearest_load::input_type& input);

logistic_regression_load::result_type
    preform_action(const logistic_regression_load& model, 
            const SVM_load::input_type& input);

boosting_load::result_type
    preform_action(const boosting_load& model, 
            const boosting_load::input_type& input);

decision_tree_load::result_type
    preform_action(const decision_tree_load& model, 
            const decision_tree_load::input_type& input);

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream&, const SVM_load&);
std::ostream& operator << (std::ostream&, const EM_load&);
std::ostream& operator << (std::ostream&, const normal_bayes_load&);
std::ostream& operator << (std::ostream&, const random_tree_load&);
std::ostream& operator << (std::ostream&, const k_nearest_load&);
std::ostream& operator << (std::ostream&, const logistic_regression_load&);
std::ostream& operator << (std::ostream&, const boosting_load&);
std::ostream& operator << (std::ostream&, const decision_tree_load&);
std::ostream& operator << (std::ostream&, const SVM_predict&);
std::ostream& operator << (std::ostream&, const EM_predict&);
std::ostream& operator << (std::ostream&, const normal_bayes_predict&);
std::ostream& operator << (std::ostream&, const random_tree_predict&);
std::ostream& operator << (std::ostream&, const k_nearest_predict&);
std::ostream& operator << (std::ostream&, const logistic_regression_predict&);
std::ostream& operator << (std::ostream&, const boosting_predict&);
std::ostream& operator << (std::ostream&, const decision_tree_predict&);


}   // end of namespace classifier

