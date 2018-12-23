#include "apps/run_experiments/verifiers_task.h"
#include "libs/experiments/ROC.h"
#include "logging.h"

actions::experiment_output
    preform_action(const AUC_ROC& , 
            const actions::experiment_input& input)
{
    using list_res = actions::experiment_output::list_items::value_type;
    using val_res = actions::experiment_output::value_items::value_type;
    LOG_ERROR<<"not supported yet AUC_ROC";
    //return actions::testing_output{1, 2, 3, 4};
    exprs::ROC calculator;
    if (calculator.classes_calculate(std::get<1>(input), std::get<0>(input))) {
        val_res area{"area", calculator.area};
        val_res rmserr{"RMSerror", calculator.rmse};
        val_res acc{"accuracy", calculator.accuracy};
        list_res pos{"sesitiviy", std::begin(calculator.sensitivity), std::end(calculator.sensitivity)};
        list_res neg{"fallout", std::begin(calculator.fall_out), std::end(calculator.fall_out)};
        return actions::experiment_output{
            actions::experiment_output::value_items{area, rmserr, acc},
            actions::experiment_output::list_items{pos, neg}
        };
    } else {
        return actions::experiment_output{};
    }

}

actions::experiment_output
    preform_action(const logistic_loss& , 
            const actions::experiment_input&)
{
    LOG_ERROR<<"not supported yet logistic_loss";
    //return actions::testing_output{45.6, 345.66, 123.55, 234.672534};
    return actions::experiment_output{};
}

actions::experiment_output
    preform_action(const kalmogorov_smirnov_test& , 
            const actions::experiment_input&)
{
    LOG_ERROR<<"not supported yet kalmogorov_smirnov_test";
    //return actions::testing_output{0.667, 0.1234, 0.0013, 33.67, 1234.876234};
    return actions::experiment_output{};
}



