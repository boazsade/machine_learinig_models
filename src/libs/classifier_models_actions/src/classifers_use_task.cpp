#include "libs/classifier_models_actions/classifers_use_task.h"
#include <iostream>

namespace classifier
{

SVM_predict::result_type
    preform_action(const SVM_predict& model, 
            const  SVM_predict::input_type&  input)
{
    return model.execute(input);
}

EM_predict::result_type
    preform_action(const EM_predict& model, 
            const EM_predict::input_type& input)
{
    return model.execute(input);
}

normal_bayes_predict::result_type
    preform_action(const normal_bayes_predict& model, 
            const SVM_predict::input_type& input)
{
    return model.execute(input);
}

random_tree_predict::result_type
    preform_action(const random_tree_predict& model, 
            const random_tree_predict::input_type& input)
{
    return model.execute(input);
}

k_nearest_predict::result_type
    preform_action(const k_nearest_predict& model, 
            const k_nearest_predict::input_type& input)
{
    return model.execute(input);
}

logistic_regression_predict::result_type
    preform_action(const logistic_regression_predict& model, 
            const SVM_predict::input_type& input)
{
    return model.execute(input);
}

boosting_predict::result_type
    preform_action(const boosting_predict& model, 
            const boosting_predict::input_type& input)
{
    return model.execute(input);
}

decision_tree_predict::result_type
    preform_action(const decision_tree_predict& model, 
            const decision_tree_predict::input_type& input)
{
    return model.execute(input);
}

///////////////////////////////////////////////////////////////////////////////

SVM_load::result_type
    preform_action(const SVM_load& model, 
            const  SVM_load::input_type&  input)
{
    return model.execute(input);
}

EM_load::result_type
    preform_action(const EM_load& model, 
            const EM_load::input_type& input)
{
    return model.execute(input);
}

normal_bayes_load::result_type
    preform_action(const normal_bayes_load& model, 
            const SVM_load::input_type& input)
{
    return model.execute(input);
}

random_tree_load::result_type
    preform_action(const random_tree_load& model, 
            const random_tree_load::input_type& input)
{
    return model.execute(input);
}

k_nearest_load::result_type
    preform_action(const k_nearest_load& model, 
            const k_nearest_load::input_type& input)
{
    return model.execute(input);
}

logistic_regression_load::result_type
    preform_action(const logistic_regression_load& model, 
            const SVM_load::input_type& input)
{
    return model.execute(input);
}

boosting_load::result_type
    preform_action(const boosting_load& model, 
            const boosting_load::input_type& input)
{
    return model.execute(input);
}

decision_tree_load::result_type
    preform_action(const decision_tree_load& model, 
            const decision_tree_load::input_type& input)
{
    return model.execute(input);
}

std::ostream& operator << (std::ostream& os, const SVM_load& ml)
{
    return os<<"SVM classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const EM_load& ml)
{
    return os<<"EM classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_load& ml)
{
    return os<<"normal bayes classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const random_tree_load& ml)
{
    return os<<"random_tree classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const k_nearest_load& ml)
{
    return os<<"k k_nearest classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_load& ml)
{
    return os<<"logistic_regression classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const boosting_load& ml)
{
    return os<<"boost classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const decision_tree_load& ml)
{
    return os<<"decision_tree classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const SVM_predict& ml)
{
    return os<<"SVM classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const EM_predict& ml)
{
    return os<<"EM classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_predict& ml)
{
    return os<<"normal_bayes classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const random_tree_predict& ml)
{
    return os<<"random_tree classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const k_nearest_predict& ml)
{
    return os<<"k nearst classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_predict& ml)
{
    return os<<"logistic_regression classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const boosting_predict& ml)
{
    return os<<"boost classifier "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const decision_tree_predict& ml)
{
    return os<<"decision_tree classifier "<<to_string(ml);
}

}   // end of namespace classifier

