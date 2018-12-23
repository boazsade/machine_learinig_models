#pragma once
#include "regression_model_interface.h"
#include "libs/model_datatypes/model_datatypes.h"
#include <iosfwd>

namespace regression
{

MLP_train::result_type
    preform_action(const MLP_train& model, 
            const  actions::extend_train_input&  input);

SVM_train::result_type
    preform_action(const SVM_train& model, 
            const  actions::extend_train_input&  input);

logistic_regression_train::result_type
    preform_action(const logistic_regression_train& model, 
            const actions::extend_train_input& input);

////////////////////////////////////////////////////////////////////

MLP_train_sv::result_type
    preform_action(const MLP_train_sv& model, 
            const  actions::extend_train_input&  input);

SVM_train_sv::result_type
    preform_action(const SVM_train_sv& model, 
            const  actions::extend_train_input&  input);

logistic_regression_train_sv::result_type
    preform_action(const logistic_regression_train_sv& model, 
            const actions::extend_train_input& input);

////////////////////////////////////////////////////////////////////
MLP_test::result_type
    preform_action(const MLP_test& model, 
            const actions::testing_input& input);

logistic_regression_test::result_type
    preform_action(const logistic_regression_test& model, 
            const actions::testing_input& input);

SVM_test::result_type
    preform_action(const SVM_test& model, 
            const actions::testing_input& input);

////////////////////////////////////////////////////////////////////

MLP_test_sv::result_type
    preform_action(const MLP_test_sv& model, 
            const actions::testing_input& input);

logistic_regression_test_sv::result_type
    preform_action(const logistic_regression_test_sv& model, 
            const actions::testing_input& input);

SVM_test_sv::result_type
    preform_action(const SVM_test_sv& model, 
            const actions::testing_input& input);

////////////////////////////////////////////////////////////////////
MLP_save::result_type
    preform_action(const MLP_save& model, 
            const MLP_save::input_type& input);

logistic_regression_save::result_type
    preform_action(const logistic_regression_save& model, 
            const logistic_regression_save::input_type& input);

SVM_save::result_type
    preform_action(const SVM_save& model, 
            const SVM_save::input_type& input);


////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream&, const SVM_train&);
std::ostream& operator << (std::ostream&, const logistic_regression_train&);
std::ostream& operator << (std::ostream&, const MLP_test&);
std::ostream& operator << (std::ostream&, const MLP_train&);
std::ostream& operator << (std::ostream&, const logistic_regression_test&);
std::ostream& operator << (std::ostream&, const SVM_test&);

std::ostream& operator << (std::ostream&, const SVM_train_sv&);
std::ostream& operator << (std::ostream&, const logistic_regression_train_sv&);
std::ostream& operator << (std::ostream&, const MLP_test_sv&);
std::ostream& operator << (std::ostream&, const MLP_train_sv&);
std::ostream& operator << (std::ostream&, const logistic_regression_test_sv&);
std::ostream& operator << (std::ostream&, const SVM_test_sv&);

std::ostream& operator << (std::ostream&, const MLP_save&);
std::ostream& operator << (std::ostream&, const logistic_regression_save&);
std::ostream& operator << (std::ostream&, const SVM_save&);
}       // end of namespace regression

