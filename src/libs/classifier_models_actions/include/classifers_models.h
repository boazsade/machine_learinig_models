#pragma once
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/model_datatypes/model_base.h"
#include "libs/ml_models/support_vector_machine.h"
#include "libs/ml_models/EM.h"
#include "libs/ml_models/normal_bayes_classifier.h"
#include "libs/ml_models/logistic_regression.h"
#include "libs/ml_models/random_trees.h"
#include "libs/ml_models/decision_trees.h"
#include "libs/ml_models/K_nearest.h"
#include "libs/ml_models/boosting.h"
#include "libs/ml_models/MLP.h"
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

struct null_args{};
struct undefined_model{};

struct SVM : model_base
{
    using model = mlmodels::classifier_svm;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

   prediction_value predict(model_type model, const row_type& row) const;

    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    SVM() = default;

    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const SVM& svm);

struct EM : model_base
{
    using model = mlmodels::expectation_maximization;
    using model_type = model::model_type;
    using covariance_matrices_t = model::args::covariance_matrices_t;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    prediction_value predict(model_type model, const row_type& row) const;

    EM() = default;

    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const EM& m);

struct normal_bayes : model_base
{
    using model = mlmodels::normal_bayes_classifier;
    using model_type = model::model_type;
    using args = null_args;

    model_type train(const training_data& input,
            const labels_type& lablels, args) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    prediction_value predict(model_type model, const row_type& row) const;


    normal_bayes() = default;


    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};

std::ostream& operator << (std::ostream& os, const normal_bayes& nb);

struct k_nearest : model_base
{
    using model = mlmodels::K_nearest;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;
    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    prediction_value predict(model_type model, const row_type& row) const;


    k_nearest() = default;


    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }

};
std::ostream& operator << (std::ostream& os, const k_nearest& kn);

struct random_tree : model_base
{
    using model = mlmodels::random_trees;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;

    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;
    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    prediction_value predict(model_type model, const row_type& row) const;


    random_tree() = default;


    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const random_tree& rtree);

struct logistic_regration : model_base
{
    using model = mlmodels::classifier_LG_model;
    using model_type = model::model_type;;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels,
            const args& a) const;

    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    prediction_value predict(model_type model, const row_type& row) const;


    logistic_regration() = default;


    bool save(model_type m, const std::string& at) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const logistic_regration& log_reg);


struct boosting_model : model_base
{
    using model = mlmodels::boosting;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;

    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;
    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    boosting_model() = default;

    bool save(model_type m, const std::string& at) const;

    prediction_value predict(model_type model, const row_type& row) const;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const boosting_model& b);

struct decision_tree : model_base
{
    using model = mlmodels::decision_trees;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

    test_output test(model_type model, 
            const testing_input& data, int classes) const;

    bool save(model_type m, const std::string& at) const;

    prediction_value predict(model_type model, const row_type& row) const;

    decision_tree() = default;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const decision_tree& b);

////////////////////////////////////////////////////////////////////
}   // end of namespace classifier

