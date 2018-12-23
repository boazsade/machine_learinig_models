#pragma once

#include "libs/ml_models/general_types.h"
#include <tuple>

namespace actions
{

using training_data = mlmodels::models_input;
using model_samples = std::tuple<training_data,                 // samples 
                                 mlmodels::class_data>;       // labels
using training_input = model_samples;
using testing_input = mlmodels::models_input;
using testing_output = mlmodels::testing_result;
using experiment_input = std::tuple<mlmodels::class_data,     // from the prediction
                                    mlmodels::class_data>;    // actual data

struct extend_train_input 
{
    training_input data;
    int            class_count = -1;

    extend_train_input() = default;
    extend_train_input(const training_input& ti, int cc) :
        data{ti}, class_count{cc}
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
//          helper function to retrive data
constexpr struct samples_index {
    enum {
        SAMPLES = 0,
        LABELS = 1
    };
} _samples_index = samples_index{};

constexpr struct expers_index {
    enum {
        PREDICTIONS = 0,
        MEASURE = 1
    };
} _expers_index = expers_index{};

///////////////////////////////////////////////////////////////////////////////
//
constexpr inline 
const std::tuple_element<samples_index::SAMPLES, model_samples>::type&
    samples_data(const model_samples& ms) {
        return std::get<samples_index::SAMPLES>(ms);
}

constexpr inline 
std::tuple_element<samples_index::SAMPLES, model_samples>::type&
    samples_data(model_samples& ms) {
        return std::get<samples_index::SAMPLES>(ms);
}

constexpr inline 
const std::tuple_element<samples_index::LABELS, model_samples>::type&
    labels_data(const model_samples& ms) {
        return std::get<samples_index::LABELS>(ms);
}

constexpr inline 
std::tuple_element<samples_index::LABELS, model_samples>::type&
    labels_data(model_samples& ms) {
        return std::get<samples_index::LABELS>(ms);
}


constexpr inline 
const std::tuple_element<samples_index::SAMPLES, model_samples>::type&
    samples_data(const extend_train_input& ms) {
        return std::get<samples_index::SAMPLES>(ms.data);
}

constexpr inline 
std::tuple_element<samples_index::SAMPLES, model_samples>::type&
    samples_data(extend_train_input& ms) {
        return std::get<samples_index::SAMPLES>(ms.data);
}

constexpr inline 
const std::tuple_element<samples_index::LABELS, model_samples>::type&
    labels_data(const extend_train_input& ms) {
        return std::get<samples_index::LABELS>(ms.data);
}

constexpr inline 
std::tuple_element<samples_index::LABELS, model_samples>::type&
    labels_data(extend_train_input& ms) {
        return std::get<samples_index::LABELS>(ms.data);
}

constexpr inline int
    class_count (const extend_train_input& ms) {
        return ms.class_count;
}
///////////////////////////////////////////////////////////////////////////////

constexpr inline 
const std::tuple_element<expers_index::PREDICTIONS, experiment_input>::type&
    predictions(const experiment_input& ms) {
        return std::get<expers_index::PREDICTIONS>(ms);
}

constexpr inline 
std::tuple_element<expers_index::PREDICTIONS, experiment_input>::type&
    predictions(experiment_input& ms) {
        return std::get<expers_index::PREDICTIONS>(ms);
}

constexpr inline 
const std::tuple_element<expers_index::MEASURE, experiment_input>::type&
    measures(const experiment_input& ms) {
        return std::get<expers_index::MEASURE>(ms);
}

constexpr inline 
std::tuple_element<expers_index::MEASURE, experiment_input>::type&
    measures(experiment_input& ms) {
        return std::get<expers_index::MEASURE>(ms);
}

}   // end of namespace actions

