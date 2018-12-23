#include "libs/regression_models_actions/regressionsmodels_use_task.h"
#include <iostream>

namespace regression
{

MLP_load::result_type
    preform_action(const MLP_load& model, 
            const  MLP_load::input_type&  input)
{
    return model.execute(input);
}


SVM_load::result_type
    preform_action(const SVM_load& model, 
            const  SVM_load::input_type&  input)
{
    return model.execute(input);
}


logistic_regression_load::result_type
    preform_action(const logistic_regression_load& model, 
            const logistic_regression_load::input_type& input)
{
    return model.execute(input);
}

MLP_predict::result_type
    preform_action(const MLP_predict& model, 
            const  MLP_predict::input_type&  input)
{
    return model.execute(input);
}

SVM_predict::result_type
    preform_action(const SVM_predict& model, 
            const  SVM_predict::input_type&  input)
{
    return model.execute(input);
}

logistic_regression_predict::result_type
    preform_action(const logistic_regression_predict& model, 
            const logistic_regression_predict::input_type& input)
{
    return model.execute(input);
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& os, const SVM_predict& ml)
{
    return os<<"SVM regression "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_predict& ml)
{
    return os<<"logistic_regration regression "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const MLP_load& ml)
{
    return os<<"MLP regression "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const MLP_predict& ml)
{
    return os<<"MLP regression "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_load& ml)
{
    return os<<"logistic_regration regression "<<to_string(ml);
}

std::ostream& operator << (std::ostream& os, const SVM_load& ml)
{
    return os<<"SVM regression "<<to_string(ml);
}

}   // end of namesapce regression

