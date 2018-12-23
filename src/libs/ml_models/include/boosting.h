#pragma once

#include "general_types.h"
#include "decision_trees.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{

struct boosting_impl;

struct boosting :
    classifier_model
{
    using model_type = std::shared_ptr<boosting_impl>;
    struct args : dtree_args
    {
        enum type_t {
            DISCRETE,
            REAL,
            LOGIT,
            GENTLE
        };
        type_t type = REAL;
        int weak_count = 100;
        double weight_trim_rate = 0.95;

        using dtree_args::dtree_args;

        args() = default;

        args(type_t t, int wc, double wtr,
            int cvf, int max_cat, int max_d,
            int min_sa, bool us, predict_type p);

        static type_t string2type(const std::string& s);
    };

    boosting() = default;

    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes,
            const args& a) const;

    bool predict(model_type model,
            const testing_data& data,
            testing_result& predications) const;

    value_type predict(model_type model,
                const class_data& samples) const;


    class_data test(model_type model, 
            const testing_data& data,
            int classes) const;


};

void save_model(boosting::model_type modeli, const char* to);
}   // end of namespace clsassifier
