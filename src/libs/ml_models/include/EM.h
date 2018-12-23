#pragma once
#include "general_types.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{

struct EM_impl;

struct expectation_maximization : classifier_model
{
    using model_type = std::shared_ptr<EM_impl>;

    struct args
    {
        enum covariance_matrices_t {
            SPHERICAL,
            DIAGONAL,
            GENERIC
        };
        int cluster_count = 5;
        int min_in_cluster = 20;
        float termination_crtiria = 0.01;
        covariance_matrices_t mat_shape = covariance_matrices_t::DIAGONAL;

        args() = default;
        args(int cc, int mic, float tc,
             covariance_matrices_t ms);

        static covariance_matrices_t string2type(const std::string& from);
    };

    expectation_maximization() = default;
    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes, const args& a) const;

    bool predict(model_type model, const testing_data& data,
            testing_result& predications) const;

    class_data test(model_type model, 
            const testing_data& data, int classes) const;

    value_type predict(model_type model,
                const class_data& samples) const;

};

void save_model(expectation_maximization::model_type model,
        const char* to);

}   // end of namemspace classifier


