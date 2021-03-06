#pragma once
#include "actions_types.h"
#include "libs/scores_evaluation/verifiers.h"
#include "libs/model_datatypes/model_datatypes.h"

actions::experiment_output
    preform_action(const AUC_ROC& task, const actions::experiment_input& arg);

actions::experiment_output
    preform_action(const logistic_loss& task, const actions::experiment_input& arg);

actions::experiment_output
    preform_action(const kalmogorov_smirnov_test& task, const actions::experiment_input& arg);

actions::experiment_output
    preform_action(const MSE_test& task, const actions::experiment_input& arg);

actions::experiment_output
    preform_action(const confusion_matric_test& task, const actions::experiment_input& arg);

actions::experiment_output
    preform_action(const null_verifier& task, const actions::experiment_input& arg);

