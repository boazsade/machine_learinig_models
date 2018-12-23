#include "apps/run_experiments/classifersmodels_task.h"
#include "logging.h"
#include <tuple>
#include <iostream>

//namespace classifier
//{

SVM_train::result_type
    preform_action(const SVM_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"SVM train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}


EM_train::result_type
    preform_action(const EM_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"EM train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

normal_bayes_train::result_type
    preform_action(const normal_bayes_train& model, 
            const actions::training_input& input)
{
    return model.execute(std::get<0>(input), std::get<1>(input));
}

random_tree_train::result_type
    preform_action(const random_tree_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"random tree train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

k_nearest_train::result_type
    preform_action(const k_nearest_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"k nearest train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

logistic_regration_train::result_type
    preform_action(const logistic_regration_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"logistic regration train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

boosting_train::result_type
    preform_action(const boosting_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"boosting train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

decision_tree_train::result_type
    preform_action(const decision_tree_train& model, 
            const actions::training_input& input)
{
    LOG_DEBUG<<"desition tree train called";
    return model.execute(std::get<0>(input), std::get<1>(input));
}

///////////////////////////////////////////////////////////////////////////////
SVM_test::result_type
    preform_action(const SVM_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"SVM test called";
    return model.execute(input);
}

EM_test::result_type
    preform_action(const EM_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"EM test called";
    return model.execute(input);
}

normal_bayes_test::result_type
    preform_action(const normal_bayes_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"normal bayes test called";
    return model.execute(input);
}

random_tree_test::result_type
    preform_action(const random_tree_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"random tree test called";
    return model.execute(input);
}

k_nearest_test::result_type
    preform_action(const k_nearest_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"k nearset test called";
    return model.execute(input);
}

logistic_regration_test::result_type
    preform_action(const logistic_regration_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"logistic regration test called";
    return model.execute(input);
}

boosting_test::result_type
    preform_action(const boosting_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"boosting test called";
    return model.execute(input);
}

decision_tree_test::result_type
    preform_action(const decision_tree_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"decistion tree test called";
    return model.execute(input);
}

template<typename T> inline
std::ostream& display(std::ostream& os, T)
{
    typename T::model_impl model;
    return os<<model;
}

std::ostream& operator << (std::ostream& os,  const SVM_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regration_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const SVM_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const EM_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const normal_bayes_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regration_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_test& d)
{
    return display(os, d);
}
//}   // end of namesapce classifier
