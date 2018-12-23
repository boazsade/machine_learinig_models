#include "verifiers_task.h"
#include "logging.h"

actions::experiment_output
    preform_action(const AUC_ROC& , 
            const actions::testing_output&)
{
    LOG_ERROR<<"not supported yet AUC_ROC";
    return actions::testing_output{};
}

actions::experiment_output
    preform_action(const logistic_loss& , 
            const actions::testing_output&)
{
    LOG_ERROR<<"not supported yet logistic_loss";
    return actions::testing_output{};
}

actions::experiment_output
    preform_action(const kalmogorov_smirnov_test& , 
            const actions::testing_output&)
{
    LOG_ERROR<<"not supported yet kalmogorov_smirnov_test";
    return actions::testing_output{};
}



