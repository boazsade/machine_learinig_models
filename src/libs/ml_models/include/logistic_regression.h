#pragma once

#include "general_types.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{

struct logistic_reg_impl;

struct logistic_regression
{
    using model_type = std::shared_ptr<logistic_reg_impl>;

    struct args
    {
        enum regularization_t {
            DISABLE, REG_L1, REG_L2
        };

        enum train_method_t {
            BATCH, MIN_BATCH
        };

        double learning_rate = 0.001;
        int iterations = 1000;
        int min_batch_size = 1;
        regularization_t regular = REG_L2;
        train_method_t train_method = BATCH;

        args() = default;
        args(double lr, int iters, int min_bs,
            regularization_t r, train_method_t tm);

        static regularization_t string2reg(const std::string& from);
        static train_method_t string2train(const std::string& from);

    };


    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes,
            const args& a) const;

    bool predict(model_type model, const testing_data& data,
            testing_result& predications) const;

    value_type predict(model_type model,
                const class_data& samples) const;


protected:
    logistic_regression() = default;
};
void save_model(logistic_regression::model_type model, const char* to);

struct classifier_LG_model :
    public logistic_regression,
    classifier_model
{
    classifier_LG_model() = default;
    class_data test(model_type model, const testing_data& data,
            int classes) const;
};

struct regression_LR_model :
    public logistic_regression,
    regression_model
{
    regression_LR_model() = default;
    class_data test(model_type model, const testing_data& data,
            int classes) const;
};

}   // end of namespace mlmodels

