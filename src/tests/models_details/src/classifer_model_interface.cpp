#include "apps/run_experiments/classifer_model_interface.h"

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

