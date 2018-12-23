#pragma once
#include "general_types.h"
#include "dtree_args.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{

struct randomtree_impl;

struct random_trees : classifier_model
{
    using model_type = std::shared_ptr<randomtree_impl>;

    struct args : dtree_args
    {
        bool var_importance  = false;
        double regression_accuracy = 0.0;

        using dtree_args::dtree_args;

        args() = default;
        args(bool vi, double ra,
            int cvf, int max_cat, int max_d,
            int min_sa, bool us, predict_type p);
    };

    random_trees() = default;
    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes, const args& a) const;

    bool predict(model_type model,
            const testing_data& data,
            testing_result& predications) const;

    class_data test(model_type model, 
            const testing_data& data, int classes) const;

    value_type predict(model_type model,
                const class_data& samples) const;

};

void save_model(random_trees::model_type model, const char* to);
}   // end of namespace mlmodels

