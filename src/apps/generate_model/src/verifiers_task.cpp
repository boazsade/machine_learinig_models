#include "apps/generate_model/verifiers_task.h"
#include "libs/scores_evaluation/score_evaluation.h"
#include "logging.h"

params_output
    preform_action(const AUC_ROC& task, const actions::experiment_input& arg)
{
    LOG_INFO<<"running AUC ROC calculation on data size of "<<std::get<0>(arg).size();
    auto res = calculate(task, arg);   
    return params_output{res};
}

params_output
    preform_action(const logistic_loss& , const actions::experiment_input& )
{
    LOG_INFO<<"log loss is not supported for experiments on data!!";
    return params_output{};
}

params_output
    preform_action(const kalmogorov_smirnov_test& task, const actions::experiment_input& arg)
{
    LOG_INFO<<"running KS test on data size of "<<std::get<0>(arg).size();
    return params_output{calculate(task, arg)};
}

params_output
    preform_action(const MSE_test& task, const actions::experiment_input& arg)
{
    LOG_INFO<<"running MSE test on data size "<<std::get<0>(arg).size();
    return params_output{calculate(task, arg)};
}

params_output
    preform_action(const null_verifier& , const actions::experiment_input& )
{
    LOG_INFO<<"null operation do nothing for testing results";
    return params_output{};
}


