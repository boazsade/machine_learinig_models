#include "libs/ml_models/normal_bayes_classifier.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/core.hpp>
#include <opencv/ml.h>

namespace mlmodels
{

struct normal_bayes_impl : detail::general_model<cv::ml::NormalBayesClassifier>
{
    using base_t = detail::general_model<cv::ml::NormalBayesClassifier>;
    using base_t::base_t;

    normal_bayes_impl() : base_t{}
    {
    }

};


normal_bayes_classifier::model_type 
normal_bayes_classifier::train(const training_data& data,
        const class_data& classes) const
{
    model_type model{std::make_shared<normal_bayes_impl>()};
    if (model->class_train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}

bool normal_bayes_classifier::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}


value_type normal_bayes_classifier::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

class_data normal_bayes_classifier::test(model_type model, 
        const testing_data& data, int) const
{
    class_data res;
    if (model && !empty(data)) {
        res.reserve(rows(data));
        boost::transform(data, std::back_inserter(res), [model](auto row) {
                cv::Mat tmp_data{1, (int)row.size(), CV_32F, (void*)row.origin()};
                cv::Mat ret, preds;
                model->handle->predictProb(tmp_data, ret, preds);
                return preds.at<float>(0, 0);
            }
        );
    }
    return res;
}

void save_model(normal_bayes_classifier::model_type model, const char* path)
{
    if (path) {
        model->save(path);
    }
}

normal_bayes_classifier::model_type
    normal_bayes_classifier::load(const char* from)
{
    return model_type{std::make_shared<normal_bayes_impl>(normal_bayes_impl::load(from))};
}

}   // end of namespace mlmodels

