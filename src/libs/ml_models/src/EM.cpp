#include "libs/ml_models/EM.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/core.hpp>
#include <opencv/ml.h>
#include <boost/assign.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iostream>
//#define _PRINT_MODEL_PARAMS
#ifdef _PRINT_MODEL_PARAMS
#   include <boost/range/algorithm/copy.hpp>
#endif // _PRINT_MODEL_PARAMS
#ifdef _BYPASS_MODEL_CREATE
#   include <algorithm>
#endif  // _BYPASS_MODEL_CREATE
#include <iterator>

namespace mlmodels
{

namespace
{
int convert2internal(expectation_maximization::args::covariance_matrices_t which)
{
    switch (which) {
        case expectation_maximization::args::DIAGONAL:
            return cv::ml::EM::COV_MAT_DIAGONAL;
        case expectation_maximization::args::GENERIC:
            return cv::ml::EM::COV_MAT_GENERIC;
        case expectation_maximization::args::SPHERICAL:
            return cv::ml::EM::COV_MAT_SPHERICAL;
    }
}

}   // end of local namespace

expectation_maximization::args::args(int cc, int mic, float tc,
        covariance_matrices_t ms) :
            cluster_count(cc), min_in_cluster(mic),
            termination_crtiria(tc), mat_shape(ms)
{
}

expectation_maximization::args::covariance_matrices_t
    expectation_maximization::args::string2type(const std::string& from)
{
    static const std::map<std::string, expectation_maximization::args::covariance_matrices_t> s2t =
                boost::assign::map_list_of("spherical", expectation_maximization::args::SPHERICAL)
                                        ("diagonal", expectation_maximization::args::DIAGONAL)
                                        ("generic", expectation_maximization::args::GENERIC);
    auto i = s2t.find(from);
    if (i == std::end(s2t)) {
        throw std::runtime_error{"invalid EM matrix shape " + from};
    } else {
        return i->second;
    }
}

struct EM_impl
{
    EM_impl(cv::Ptr<cv::ml::EM> p) :
        em{p}
    {
    }

    EM_impl(float tc, int mnc, int cn, 
            expectation_maximization::args::covariance_matrices_t ms) : 
        em{cv::ml::EM::create()}
    {
        em->setClustersNumber(cn);
        em->setTermCriteria({cv::TermCriteria::COUNT + cv::TermCriteria::EPS,  mnc, tc });
        em->setCovarianceMatrixType(convert2internal(ms));
    }

    bool train(const training_data& samples,
                const class_data& 
            ) 
    {
//        std::cout<<"running training on "<<rows(samples)<<"X"<<columns(samples)<<std::endl;
        static_assert(sizeof(class_data::value_type) == sizeof(int), 
                "we cannot run this code if sizeof float is not the same as sizeof int - casting issue");
        if (empty(samples)/* || lables.empty()*/) {
            return false;
        }
        auto data(detail::prepare_samples(samples, CV_32FC1));
        cv::Mat dummy;  // we would not use this
        return em->trainEM(data, cv::noArray(), dummy, cv::noArray());
    }

    value_type predict(const class_data& samples) const
    {
        if (samples.empty()) {
            return missing_value<value_type>();
        }

        cv::Mat tmp_data{1, (int)samples.size(), CV_32F, (void*)samples.data()};
        return this->em->predict2(tmp_data, cv::noArray())[1];
    }

    bool predict(const testing_data& testInput,
            testing_result& results) const
    {
        if (testInput.empty()) {
            return false;
        }
        results.reserve(rows(testInput));
        cv::Mat out_res;
        auto rc = rows(testInput);
        results.reserve(rc);
        boost::transform(testInput, std::back_inserter(results), [this](auto row) {
                        cv::Mat tmp_data{1, (int)row.size(), CV_32F, (void*)row.origin()};
                        auto response = this->em->predict2(tmp_data, cv::noArray());
                        return static_cast<testing_result::value_type>(response[1]);//, response[0]};
                                
                }
        );
        return !results.empty();
    }

    

    class_data test(const testing_data& input) const 
    {
        auto rc = rows(input);
        class_data results;
        results.reserve(rc);
        boost::transform(input, std::back_inserter(results), [this](auto row) {
                        cv::Mat tmp_data{1, (int)row.size(), CV_32F, (void*)row.origin()};
                        auto response = this->em->predict2(tmp_data, cv::noArray());
                        return response[0];//, response[0]};
                                
                }
        );
        return results;
    }

    void save(const char* path) const
    {
        em->save(path);
    }

    static cv::Ptr<cv::ml::EM> load(const char* from) {
        return cv::ml::StatModel::load<cv::ml::EM>(from);
    }

   cv::Ptr<cv::ml::EM> em; 

};

expectation_maximization::model_type 
expectation_maximization::train(const training_data& data,
        const class_data& classes,
        const args& a) const
{
    model_type model{std::make_shared<EM_impl>(a.termination_crtiria, 
                a.min_in_cluster, a.cluster_count, a.mat_shape)};
    if (model->train(data, classes)) {
        return model;
    } else {
        static const model_type err = model_type{};
        return err;
    }
}

expectation_maximization::model_type
    expectation_maximization::load(const char* from) 
{
    return model_type{std::make_shared<EM_impl>(EM_impl::load(from))};
}

bool expectation_maximization::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}

value_type expectation_maximization::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    } else {
        return missing_value<value_type>();
    }
}

class_data expectation_maximization::test(model_type model, 
                                const testing_data& data, int) const
{
    class_data result;
    if (model && !data.empty()) {
        result = model->test(data);
    }
    return result;
}

void save_model(expectation_maximization::model_type model, const char* path)
{
    if (model) {
        model->save(path);
    }
}

}   // end of namespace mlmodels

