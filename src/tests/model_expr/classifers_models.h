#pragma once
#include "libs/classfication_algos/support_vector_machine.h"
#include "libs/classfication_algos/general_types.h"
#include <iosfwd>

// list of alogrithms that we are applying to build classifications models
// these are the target for this all application - we are feeding it with
// the train data, then run it with the test data that then is being fed
// into the experiments
// all those algorithms are getting the same type of data - a matrix
// that holds the data that the models is either build with or test with
// and a vector of data that contins either the results to train against
// or the actual result that can be compare with
namespace classifier
{

struct undefined_model{};

struct model_base
{
    using training_input = training_data;
    using testing_input = testing_data;
    using test_output = testing_result;
};

struct SVM : model_base
{
    using model_type = support_vector_machine::model_type;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;

    SVM() = default;
};

std::ostream& operator << (std::ostream& os, const SVM& svm);

struct EM : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;

    EM() = default;
};
std::ostream& operator << (std::ostream& os, const EM& m);

struct normal_bayes : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;

    normal_bayes() = default;
};

std::ostream& operator << (std::ostream& os, const normal_bayes& nb);

struct k_nearest : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;
    k_nearest() = default;

};
std::ostream& operator << (std::ostream& os, const k_nearest& kn);

struct random_tree : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;
    random_tree() = default;
};
std::ostream& operator << (std::ostream& os, const random_tree& rtree);

struct logistic_regration : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;
    logistic_regration() = default;
};
std::ostream& operator << (std::ostream& os, const logistic_regration& log_reg);

struct boosting : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;
    boosting() = default;
};
std::ostream& operator << (std::ostream& os, const boosting& b);

struct decision_tree : model_base
{
    using model_type = undefined_model;

    model_type train(const training_input& input,
            const class_data& lablels) const;
    bool test(model_type model,
            const testing_input& input,
            test_output& result) const;
    decision_tree() = default;
};
std::ostream& operator << (std::ostream& os, const decision_tree& b);

////////////////////////////////////////////////////////////////////
}   // end of namespace classifier

