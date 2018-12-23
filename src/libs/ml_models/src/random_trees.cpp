#include "libs/ml_models/random_trees.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/core.hpp>
#include <opencv/ml.h>

namespace mlmodels
{

random_trees::args::args(bool vi, double ra,
            int cvf, int max_cat, int max_d,
            int min_sa, bool us, predict_type p) :
    dtree_args{cvf, max_cat, max_d, min_sa, us, p},
    var_importance(vi), regression_accuracy(ra)
{
}

struct randomtree_impl : 
    detail::general_model<cv::ml::RTrees>
{
    using base_t = detail::general_model<cv::ml::RTrees>;
    using base_t::base_t;

    randomtree_impl(const random_trees::args& a) : base_t{}
    {
        handle->setCVFolds(a.cv_fold);
        handle->setMaxDepth(a.max_depth);
        handle->setUseSurrogates(a.use_surrogates);
        handle->setCalculateVarImportance(a.var_importance);
        handle->setMaxCategories(a.max_categories);
        handle->setMinSampleCount(a.min_samples);
    }
};

random_trees::model_type 
random_trees::train(const training_data& data,
        const class_data& classes, const args& a) const
{
    model_type model{std::make_shared<randomtree_impl>(a)};
    if (model->class_train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}

bool random_trees::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}


value_type random_trees::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

class_data random_trees::test(model_type model,
        const testing_data& data, int classes) const
{
    if (data.empty() || !model) {
        return class_data{};
    }
    return model->test(data, classes);
}

void save_model(random_trees::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}

random_trees::model_type
    random_trees::load(const char* from)
{
    return model_type{std::make_shared<randomtree_impl>(randomtree_impl::load(from))};
}

}   // end of namespace mlmodels

