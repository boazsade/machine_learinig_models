#pragma once
#include "libs/model_datatypes/model_base.h"
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/ml_models/MLP.h"
#include "libs/ml_models/logistic_regression.h"
#include "libs/ml_models/support_vector_machine.h"
#include <iosfwd>

namespace regression
{

struct MLP_model : public model_base
{
    using model = mlmodels::multi_layer_perceptrons;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;
    test_output test(model_type model,
                    const testing_input& input,
                    int) const;

    prediction_value predict(model_type model, const row_type& row) const;

    bool save(model_type m, const std::string& at) const;

    MLP_model() = default;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const MLP_model& b);

struct logistic_regration : model_base
{
    using model = mlmodels::regression_LR_model;
    using model_type = model::model_type;;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels,
            const args& a) const;

    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;
    test_output test(model_type model,
                    const testing_input& input,
                    int) const;

    bool save(model_type m, const std::string& at) const;

    prediction_value predict(model_type model, const row_type& row) const;

    logistic_regration() = default;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }
};
std::ostream& operator << (std::ostream& os, const logistic_regration& log_reg);

struct SVM : model_base
{
    using model = mlmodels::regression_svm;
    using model_type = model::model_type;
    using args = model::args;

    model_type train(const training_data& input,
            const labels_type& lablels, const args& a) const;
    bool predict(model_type model,
            const testing_input& input,
            test_output& result) const;

    test_output test(model_type model,
                    const testing_input& input,
                    int) const;

    prediction_value predict(model_type model, const row_type& row) const;

    bool save(model_type m, const std::string& at) const;

    SVM() = default;

    constexpr bool is_classifier() const {
        return mlmodels::is_classifier<model>();
    }

};
std::ostream& operator << (std::ostream& os, const SVM& svm);

}   // end of namespace regression


