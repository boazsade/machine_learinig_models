#include "libs/ml_models/K_nearest.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
//#include <iostream>


namespace mlmodels
{

namespace
{

const int K_NUMBERS = 8;

int convert(K_nearest::args::algo_type from)
{
    switch (from) {
        case K_nearest::args::BRUTE_FORCE:
            return cv::ml::KNearest::BRUTE_FORCE;
        case K_nearest::args::KDTREE:
            return cv::ml::KNearest::KDTREE;
    }
}

}   // end of namespace 

K_nearest::args::algo_type K_nearest::args::string2type(const std::string& from)
{
    if (from == "brute-force") {
        return BRUTE_FORCE;
    } else if (from == "kdtree") {
        return KDTREE;
    } else {
        throw std::runtime_error{"invalid type for k nearst algo type " + from};
    }
}

K_nearest::args::args(algo_type t, int nc) :
        type(t), nearest_count(nc)
{
}

struct knearset_impl : 
    detail::general_model<cv::ml::KNearest>
{
    using base_t = detail::general_model<cv::ml::KNearest>;
    using base_t::base_t;

    knearset_impl(const K_nearest::args& a) :
        base_t{}
    {
        handle->setIsClassifier(true);  // in run in classfication mode
        handle->setAlgorithmType(convert(a.type));
        handle->setDefaultK(a.nearest_count);
    }

    value_type predict(const class_data& data) const
    {
        auto samples{detail::prepare_samples(data)};
        cv::Mat out_res;    // we don't realy need this, but to compile
        return handle->findNearest(samples, K_NUMBERS, out_res);
    }

    bool test_special(const testing_data& data, 
            testing_result& output) const
    {
        if (data.empty()) {
            return false;
        }
        output.reserve(rows(data));
        cv::Mat out_res;
        auto samples{detail::prepare_samples(data)};
        handle->findNearest(samples, K_NUMBERS,
                        out_res);
        detail::copy_result(out_res, output, rows(data));
        return !output.empty();
    }
#if 0
    bool test_special(const testing_data& data, 
            testing_result& output, testing_result& dist,
            testing_result& nr) const
    {
        static_assert(std::is_same<testing_result::value_type, float>::value,
                "we are expecting to have floating points as types");
        if (data.empty()) {
            return false;
        }
        output.reserve(rows(data));
        auto samples{detail::prepare_samples(data)};
        for (auto i = 0; i < samples.rows; i++) {
            cv::Mat out_res;
            cv::Mat distance;
            cv::Mat neres;
            auto p = handle->findNearest(samples, K_NUMBERS,
                        out_res, neres, distance);
            std::cout<<"finish running findNearest with output result size for output: "<<out_res.size<<", distances "<<distance.size<<" and nearest result "<<neres.size<<" for K size of "<<K_NUMBERS<<" prediction at "<<i<<" = "<<p<<std::endl;
            std::cout<<"results:\n"<<out_res<<"\ndistance:\n"<<distance<<"\nnear results:\n"<<neres<<std::endl;
            //detail::copy_result(out_res, output, rows(data));
            output.push_back(p);
            detail::copy_result(distance, dist, /*rows(data) **/ K_NUMBERS);
            detail::copy_result(neres, nr, /*rows(data) */ K_NUMBERS);
        }
        return !output.empty();
    }
#endif
};

K_nearest::model_type 
K_nearest::train(const training_data& data,
        const class_data& classes,
        const args& a) const
{
    model_type model{std::make_shared<knearset_impl>(a)};
    if (model->class_train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}

K_nearest::model_type 
    K_nearest::load(const char* from) 
{
    return model_type{std::make_shared<knearset_impl>(knearset_impl::load(from))};
}

bool K_nearest::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->test_special(data, predications);
    } else {
        return false;
    }
}

class_data K_nearest::test(model_type model, 
        const testing_data& data, int ) const
{
    if (model && !data.empty()) {
        class_data output;
        if (model->test_special(data, output)) {
            return output;
        } 
    }
    return class_data{};
}

value_type K_nearest::predict(model_type model,
                const class_data& samples) const
{
    if (model && not samples.empty()) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

void save_model(K_nearest::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}
#if 0
bool K_nearest::test(model_type m, const testing_data& d,
            class_data& res, class_data& dist, class_data& nr) const
{
    if (m && !d.empty()) {
        return m->test_special(d, res, dist, nr);
    } else {
        return false;
    }
}
#endif

}   // end of namespace classfiier


