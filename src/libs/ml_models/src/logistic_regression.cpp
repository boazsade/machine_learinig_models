#include "libs/ml_models/logistic_regression.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include <boost/assign.hpp>
#include <map>

namespace mlmodels
{

logistic_regression::args::args(double lr, int iters,
        int min_bs, regularization_t r, train_method_t tm) :
    learning_rate(lr), iterations(iters), min_batch_size(min_bs),
    regular(r), train_method(tm)
{
}

logistic_regression::args::regularization_t 
    logistic_regression::args::string2reg(const std::string& from)
{
    static const std::map<std::string, regularization_t> mapper =
        boost::assign::map_list_of("disable", regularization_t::DISABLE)
                        ("reg_l1", regularization_t::REG_L1)
                        ("reg_l2", regularization_t::REG_L2);

    auto i = mapper.find(from);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid entry for logistic regaression; " + from};
    } else {
        return i->second;
    }

}

logistic_regression::args::train_method_t 
    logistic_regression::args::string2train(const std::string& from)
{
    static const std::map<std::string, train_method_t> mapper =
        boost::assign::map_list_of("batch", BATCH)
                        ("min-batch", MIN_BATCH);
    auto i = mapper.find(from);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid logistic regression method " + from};
    }
    return i->second;

}

cv::ml::LogisticRegression::RegKinds 
    convert(logistic_regression::args::regularization_t from);

cv::ml::LogisticRegression::Methods
    convert(logistic_regression::args::train_method_t from);

cv::ml::LogisticRegression::RegKinds 
    convert(logistic_regression::args::regularization_t from)
{
    switch (from) {
        case logistic_regression::args::DISABLE:
           return  cv::ml::LogisticRegression::REG_DISABLE;
        case logistic_regression::args::REG_L1:
            return cv::ml::LogisticRegression::REG_L1;
        case logistic_regression::args::REG_L2:
           return  cv::ml::LogisticRegression::REG_L2;
    }
}

cv::ml::LogisticRegression::Methods
    convert(logistic_regression::args::train_method_t from)
{
    switch (from) {
        case logistic_regression::args::BATCH:
            return cv::ml::LogisticRegression::BATCH;
        case logistic_regression::args::MIN_BATCH:
            return cv::ml::LogisticRegression::MINI_BATCH;
    }
}

struct logistic_reg_impl : detail::general_model<cv::ml::LogisticRegression>
{
    using base_t = detail::general_model<cv::ml::LogisticRegression>;
    using base_t::base_t;
    logistic_reg_impl(const logistic_regression::args& args) : 
        base_t{}
    {
        handle->setLearningRate(args.learning_rate);
        handle->setIterations(args.iterations);
        handle->setMiniBatchSize(args.min_batch_size);
        handle->setRegularization(convert(args.regular));
        handle->setTrainMethod(convert(args.train_method));
    }

    bool train(const training_data& samples,
            const class_data& lables)
    {
        return base_t::class_train(samples, lables, CV_32F, CV_32F);
    }
    

};  

logistic_regression::model_type
    logistic_regression::load(const char* from)
{
    return model_type{std::make_shared<logistic_reg_impl>(logistic_reg_impl::load(from))};
}

logistic_regression::model_type 
logistic_regression::train(const training_data& data,
        const class_data& classes, const args& a) const
{
    model_type model{std::make_shared<logistic_reg_impl>(a)};
    if (model->train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}


value_type logistic_regression::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

bool logistic_regression::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}

class_data classifier_LG_model::test(model_type model, 
        const testing_data& data, int classes) const
{
    if (model && !data.empty()) {
        return model->test(data, classes);
    }
    return class_data{};
}

class_data regression_LR_model::test(model_type model, 
        const testing_data& data, int ) const
{
    if (model && !data.empty()) {
        return model->test(data, -1);
    }
    return class_data{};
}

void save_model(logistic_regression::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}

}   // end of namespace mlmodels

