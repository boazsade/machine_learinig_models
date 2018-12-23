#pragma once
#include "verifiers.h"
#include "actions_types.h"

// action to run when verifying the result from testing
actions::experiment_output
    preform_action(const AUC_ROC& task, const actions::testing_output&);

actions::experiment_output
    preform_action(const logistic_loss& task, const actions::testing_output&);

actions::experiment_output
    preform_action(const kalmogorov_smirnov_test& task, const actions::testing_output&);


