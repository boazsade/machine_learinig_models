#include "apps/run_experiments/verifiers_task.h"
#include "apps/run_experiments/experiment_report.h"
#include "libs/scores_evaluation/score_evaluation.h"
#include "logging.h"

actions::experiment_output
    preform_action(const AUC_ROC& task, const actions::experiment_input& arg)
{
    using list_res = actions::experiment_output::list_items::value_type;
    using val_res = actions::experiment_output::value_items::value_type;

    LOG_INFO<<"running AUC test on data set of "<<std::get<0>(arg).size();
    auto res = calculate(task, arg);
        val_res area{"area", res.area};
        val_res rmserr{"RMSerror", res.rmse};
        val_res acc{"accuracy", res.accuracy};
        list_res pos{"sesitiviy", std::begin(res.sensitivity), std::end(res.sensitivity)};
        list_res neg{"fallout", std::begin(res.fall_out), std::end(res.fall_out)};
        return actions::experiment_output{
            actions::experiment_output::value_items{area, rmserr, acc},
            actions::experiment_output::list_items{pos, neg}
        };
    return actions::experiment_output{};
}

actions::experiment_output
    preform_action(const logistic_loss& , const actions::experiment_input& )
{
    LOG_INFO<<"log loss: bypassed - no action";
    return actions::experiment_output{};
}

actions::experiment_output
    preform_action(const kalmogorov_smirnov_test& task, const actions::experiment_input& arg)
{
    using val_res = actions::experiment_output::value_items::value_type;
    auto res = calculate(task, arg);
    LOG_INFO<<"reasult for KS test: P value: "<<res.P_value<<" and score: "<<res.score;
    val_res p{"P-value", res.P_value};
    val_res s{"score", res.score};
    return actions::experiment_output{
                actions::experiment_output::value_items{p, s},
                actions::experiment_output::list_items{}
    };
}

actions::experiment_output
    preform_action(const MSE_test& task, const actions::experiment_input& arg)
{
    using val_res = actions::experiment_output::value_items::value_type;

    if (std::get<0>(arg).empty() || std::get<1>(arg).empty()) {
        LOG_ERROR<<"no data for MSE calculation was given";
        return actions::experiment_output{};
    }
    LOG_INFO<<"running MSE test on data set of "<<std::get<0>(arg).size();

    auto res = calculate(task, arg);
    LOG_INFO<<"got result of "<<res.value<<" from MSE verification";
    val_res e{"mean-standard-error", res.value};
    return actions::experiment_output{
        actions::experiment_output::value_items{e},
        actions::experiment_output::list_items{}
    };
}

actions::experiment_output
    preform_action(const null_verifier& , const actions::experiment_input& )
{
    LOG_INFO<<"calling verification when this should be passed - do nothing";
   return actions::experiment_output{};
}

actions::experiment_output
    preform_action(const confusion_matric_test& task, const actions::experiment_input& arg)
{
    using val_res = actions::experiment_output::value_items::value_type;
    if (std::get<0>(arg).empty() || std::get<1>(arg).empty()) {
        LOG_ERROR<<"no data for confusion matric calculation was given";
        return actions::experiment_output{};
    }
    LOG_INFO<<"running confusion matric test on data set of "<<std::get<0>(arg).size();
    auto res = calculate(task, arg);
    if (res.false_neg) {
        LOG_INFO<<"we have output of "<<res;
    }
    val_res tp{"true-pos", static_cast<mlmodels::value_type>(res.true_pos)};
    val_res tn{"true-neg", static_cast<mlmodels::value_type>(res.true_neg)};
    val_res fn{"false-neg", static_cast<mlmodels::value_type>(res.false_neg)};
    val_res fp{"false-pos", static_cast<mlmodels::value_type>(res.false_pos)};
    return actions::experiment_output{
        actions::experiment_output::value_items{tp, tn, fn, fp},
        actions::experiment_output::list_items{}
    };
}
