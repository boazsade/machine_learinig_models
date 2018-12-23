#pragma once
#include "verifiers.h"
#include "libs/model_datatypes/model_datatypes.h"
//#include "actions_types.h"

// action to run when verifying the result from testing
AUC_ROC::result_type
    calculate(const AUC_ROC& task, const actions::experiment_input&);

logistic_loss::result_type
    calculate(const logistic_loss& task, const actions::experiment_input&);

kalmogorov_smirnov_test::result_type
    calculate(const kalmogorov_smirnov_test& task, const actions::experiment_input&);

MSE_test::result_type
    calculate(const MSE_test& task, const actions::experiment_input&);

confusion_matric_test::result_type
    calculate(const confusion_matric_test& task, const actions::experiment_input&);

null_verifier::result_type
    calculate(const null_verifier& task, const actions::experiment_input&);
