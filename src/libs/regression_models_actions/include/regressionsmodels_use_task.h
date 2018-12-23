#pragma once
#include "regression_model_use.h"
#include "libs/model_datatypes/model_datatypes.h"
#include <iosfwd>

namespace regression
{

///////////////////////////////////////////////////////////////////////////////
//  run load
MLP_load::result_type
    preform_action(const MLP_load& model, 
            const  MLP_load::input_type&  input);

SVM_load::result_type
    preform_action(const SVM_load& model, 
            const  SVM_load::input_type&  input);

logistic_regression_load::result_type
    preform_action(const logistic_regression_load& model, 
            const logistic_regression_load::input_type& input);

///////////////////////////////////////////////////////////////////////////////
//  run prediction
MLP_predict::result_type
    preform_action(const MLP_predict& model, 
            const  MLP_predict::input_type&  input);

SVM_predict::result_type
    preform_action(const SVM_predict& model, 
            const  SVM_predict::input_type&  input);

logistic_regression_predict::result_type
    preform_action(const logistic_regression_predict& model, 
            const logistic_regression_predict::input_type& input);

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream&, const SVM_predict&);
std::ostream& operator << (std::ostream&, const logistic_regression_predict&);
std::ostream& operator << (std::ostream&, const MLP_load&);
std::ostream& operator << (std::ostream&, const MLP_predict&);
std::ostream& operator << (std::ostream&, const logistic_regression_load&);
std::ostream& operator << (std::ostream&, const SVM_load&);


}   // end of namespace regression


