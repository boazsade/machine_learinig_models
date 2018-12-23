#pragma once
#include "libs/ml_models/general_types.h"
#include "utils/missing_values.hpp"
#include <boost/range/algorithm/transform.hpp>
#include <opencv2/core.hpp>
#include <opencv/ml.h>
#include <cstdint>
#include <limits>

namespace mlmodels
{

namespace detail
{


void copy_result(const cv::Mat& from, testing_result& to,
        std::size_t expected_size);

class_data copy_result(const cv::Mat& from, std::size_t expected_size);

cv::Mat prepare_samples(const training_data& from, int type = CV_32F);
cv::Mat prepare_samples(const class_data& from, int type = CV_32F);
cv::Mat prepare_labels(const classes_data& from, int type = CV_32SC1);
cv::Mat prepare_labels(const class_data& from, int type = CV_32F);
class_data::value_type predict_prob(class_data::value_type from);

template<typename ModelType>
struct general_model
{
    using model_t = ModelType;

    general_model() :
        handle{model_t::create()}
    {
    }

    general_model(cv::Ptr<model_t> m) :
        handle{m}
    {
    }

    bool class_train(const training_data& samples,
            const class_data& lables, int stype = CV_32F,
            int ctype = CV_32SC1) 
    {
        return this->train(samples, lables, stype, ctype);
    }

    bool reg_train(const training_data& samples,
            const class_data& lables, int stype = CV_32F,
            int ctype = CV_32F) 
    {
        return this->train(samples, lables, stype, ctype);
    }

    bool predict(const testing_data& testInput,
            testing_result& results) const
    {
        if (testInput.empty()) {
            return false;
        }
        results.reserve(rows(testInput));
        cv::Mat out_res;
        auto data(prepare_samples(testInput));
        handle->predict(data, out_res);
        copy_result(out_res, results, rows(testInput)); 
        return !results.empty();
    }


    value_type predict(const class_data& samples) const
    {
        if (samples.empty()) {
            return missing_value<value_type>();
        }
        auto data(prepare_samples(samples));
        return handle->predict(data);
    }

    class_data test(const testing_data& testInput, int classes) const
    {
        static_assert(std::is_same<value_type, class_data::value_type>::value, 
                "we want ot return from here floating point value");
        const auto test_bin_classes = [this](auto&& res, const auto& ti) {
            boost::transform(ti, std::back_inserter(res), [this](auto row) {
                        cv::Mat tmp_data{1, (int)row.size(), CV_32F, (void*)row.origin()};
                        const auto p = this->handle->predict(tmp_data, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
                        return predict_prob(p);
                    }
                );
            return res;
        };

        const auto test_any = [this] (auto&& res, const auto& ti) {
                    boost::transform(ti, std::back_inserter(res), [this](auto row) {
                        cv::Mat tmp_data{1, (int)row.size(), CV_32F, (void*)row.origin()};
                        cv::Mat ret;
                        this->handle->predict(tmp_data, ret,
                                        cv::ml::StatModel::RAW_OUTPUT);
                        return ret.at<float>(0, 0);;
                    }
                );
                return res;
        };

        class_data result;
        if (!empty(testInput)) {
            result.reserve(rows(testInput));
            if (classes == 2) {
                return test_bin_classes(result, testInput);
            } else {
                return test_any(result, testInput);
            }
        }
        return result;
    }

    void save(const char* path) const
    {
        handle->save(path);
    }

    static cv::Ptr<model_t> load(const char* path) 
    {
        return cv::ml::StatModel::load<model_t>(path);
    }


    cv::Ptr<model_t> handle;
private:
    bool train(const training_data& samples,
            const class_data& lables, int stype,
            int ctype) 
    {
        if (empty(samples) || lables.empty()) {
            return false;
        }
        auto data(prepare_samples(samples, stype));
        auto classes(prepare_labels(lables, ctype));
        cv::Ptr<cv::ml::TrainData> train_p = 
            cv::ml::TrainData::create(data, cv::ml::SampleTypes::ROW_SAMPLE, classes);
        return handle->train(train_p);
    }
};

}   // end of namespace detail
}   // end of namespace clsasifier

