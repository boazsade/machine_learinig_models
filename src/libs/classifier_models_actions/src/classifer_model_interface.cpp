#include "libs/classifier_models_actions/classifer_model_interface.h"
#if 0
EM_train::EM_train(int cc, int mic, float tc,
            covariance_matrices_t ms) : args{cc, mic, tc, ms}
{
}

EM_train::result_type EM_train::execute(const input_type& input,
            const classifier::classes_data& labels) const
{
    classifier::EM trainer;
    auto m = trainer.train(input, labels, args);
    return result_type{std::move(m)};
}

logistic_regression_train::logistic_regression_train(model_args a) :
    args(std::move(a))

{
}

logistic_regression_train::result_type logistic_regression_train::execute(const input_type& input,
            const classifier::classes_data& labels) const
{
    classifier::logistic_regration trainer;
    auto m = trainer.train(input, labels, args);
    return result_type{std::move(m)};
}
#endif
