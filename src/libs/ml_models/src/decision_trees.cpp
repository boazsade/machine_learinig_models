#include "libs/ml_models/decision_trees.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv/ml.h>
#include <opencv2/core.hpp>

namespace mlmodels
{

struct decisiontrees_impl :
    detail::general_model<cv::ml::DTrees>
{
    using base_t = detail::general_model<cv::ml::DTrees>;
    using base_t::base_t;

    decisiontrees_impl(const decision_trees::args& a) :
        base_t{}
    {
        handle->setCVFolds(a.cv_fold);
        handle->setMaxDepth(a.max_depth);
        handle->setMaxCategories(a.max_categories);
        handle->setMinSampleCount(a.min_samples);
        handle->setUseSurrogates(a.use_surrogates);
    }
};

decision_trees::model_type decision_trees::train(const training_data& data,
            const class_data& classes, const args& a) const
{
    model_type model{std::make_shared<decisiontrees_impl>(a)};

    if (model->class_train(data, classes)) {
        return model;
    } else {
        return model_type{};
    }
}

bool decision_trees::predict(model_type model,
            const testing_data& data,
            testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}

decision_trees::model_type
    decision_trees::load(const char* from)
{
    return model_type{std::make_shared<decisiontrees_impl>(decisiontrees_impl::load(from))};
}

value_type decision_trees::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

class_data  decision_trees::test(model_type model, 
        const testing_data& data, int classes) const
{
    if (model) {
        return model->test(data, classes);
    } else {
        return class_data{};
    }
}

void save_model(decision_trees::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}

}   // end of namespace mlmodels

