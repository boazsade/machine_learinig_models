#include "libs/ml_models/boosting.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/ml.hpp>
#include <boost/assign.hpp>
#include <map>

namespace mlmodels
{

namespace
{

int convert(boosting::args::type_t from)
{
    switch (from) {
        case boosting::args::GENTLE:
            return cv::ml::Boost::GENTLE;
        case boosting::args::DISCRETE:
            return cv::ml::Boost::DISCRETE;
        case boosting::args::REAL:
            return cv::ml::Boost::REAL;
        case boosting::args::LOGIT:
            return cv::ml::Boost::LOGIT;
    }

}

}   // end of local namespace

boosting::args::type_t boosting::args::string2type(const std::string& s)
{
    static const std::map<std::string, type_t> mapper = 
        boost::assign::map_list_of("discrete", DISCRETE)
                                  ("real", REAL)
                                  ("logit", LOGIT)
                                  ("gentle", GENTLE);
    auto i = mapper.find(s);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid boosting type " + s};
    } else {
        return i->second;
    }
}

const auto DEFAULT_CV_FOLD_VAL = 1;

struct boosting_impl :
    detail::general_model<cv::ml::Boost>
{
    using base_t = detail::general_model<cv::ml::Boost>;
    using base_t::base_t;

    boosting_impl(const boosting::args& a) : base_t{}
    {
        auto cvf = a.cv_fold; 
        if (cvf > DEFAULT_CV_FOLD_VAL) {
            cvf = DEFAULT_CV_FOLD_VAL;
        }
        handle->setWeakCount(a.weak_count);
        handle->setWeightTrimRate(a.weight_trim_rate);
        handle->setBoostType(convert(a.type));
        handle->setCVFolds(cvf);
        handle->setMaxDepth(a.max_depth);
        handle->setUseSurrogates(a.use_surrogates);
    }
};


boosting::args::args(type_t t, int wc, double wtr,
            int cvf, int max_cat, int max_d,
            int min_sa, bool us,  predict_type p) :
                dtree_args{ cvf, max_cat, max_d, min_sa, us, p},
                type(t), weak_count(wc), weight_trim_rate(wtr)
{

}


boosting::model_type 
boosting::train(const training_data& data,
        const class_data& classes, const args& a) const
{
    model_type model{std::make_shared<boosting_impl>(a)};
    if (model->class_train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}

bool boosting::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}

boosting::model_type
    boosting::load(const char* from)
{
    return model_type{std::make_shared<boosting_impl>(boosting_impl::load(from))};
}

value_type boosting::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

class_data boosting::test(const model_type model, 
        const testing_data& data, int classes) const
{
    if (model && !data.empty()) {
        return model->test(data, classes);
    }
    return class_data{};
}

void save_model(boosting::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}

}   // end of namespace mlmodels

