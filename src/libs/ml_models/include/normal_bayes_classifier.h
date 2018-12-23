#pragma once
#include "general_types.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{

struct normal_bayes_impl;

struct normal_bayes_classifier :
    classifier_model
{
    using model_type = std::shared_ptr<normal_bayes_impl>;

    normal_bayes_classifier() = default;

    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes) const;

    bool predict(model_type model,
            const testing_data& data,
            testing_result& predications) const;

    class_data test(model_type model, const testing_data& data, int) const;


    value_type predict(model_type model,
                const class_data& samples) const;

};

void save_model(normal_bayes_classifier::model_type model, const char* to);
}   // end of namespace mlmodels
