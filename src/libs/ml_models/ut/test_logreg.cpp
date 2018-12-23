#include "test_data.hpp"
#include "../src/shared_operations.hpp"
#include "logging.h"
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/logistic_regression.h"
#include <opencv/cxcore.h>
#include <iostream>
//#define TEST_CODE_FOR_MODEL_LOG_REG
#ifdef TEST_CODE_FOR_MODEL_LOG_REG
#include <map>

using forward_m = std::map<int, int>;


bool set_label_map(const cv::Mat &_labels_i, forward_m& fm);
cv::Mat remap_labels(const cv::Mat& _labels_i, const forward_m& lmap);

void simulate(cv::Mat _labels_i)
{
    //std::cout<<"the type of the matrix that we are working with is "<<_labels_i<<std::endl;
    forward_m forward_mapper;    
    /*for (auto i = 0; i < _labels_i.rows; ++i) {
        float t = _labels_i.at<float>(i, 0);
        std::cout<< " ["<<i<<"] = "<<t;
    }
    std::cout<<std::endl;*/
#if 0
        auto ii = t + 0.123f;
        //std::cout<<"t "<<t<<", ii "<<ii<<", ";
        _labels_i.at<float>(i, 0) = ii;
    }
    std::cout<<std::endl;
#endif
    set_label_map(_labels_i, forward_mapper);
    cv::Mat labels_l = remap_labels(_labels_i, forward_mapper);
    int num_classes = (int) forward_mapper.size();
    if(num_classes < 2)
    {
        std::cerr<<"number of classes must be at least 2\n";
    } else {
        std::cout<<"number of classes is "<<num_classes<<std::endl;
    }
}

cv::Mat remap_labels(const cv::Mat& _labels_i, const forward_m& lmap)
{
    cv::Mat labels;
    _labels_i.convertTo(labels, CV_32S);

    cv::Mat new_labels = cv::Mat::zeros(labels.rows, labels.cols, labels.type());

    for(int i =0;i<labels.rows;i++) {
        new_labels.at<int>(i,0) = lmap.find(labels.at<int>(i,0))->second;
    }
    return new_labels;
}

bool set_label_map(const cv::Mat &_labels_i, forward_m& forward_mapper)
{
    // this function creates two maps to map user defined labels to program friendly labels two ways.
    int ii = 0;
    cv::Mat labels;

    auto labels_o = cv::Mat(0,1, CV_8U);
    auto labels_n = cv::Mat(0,1, CV_8U);

    _labels_i.convertTo(labels, CV_32S);

    for(int i = 0;i<labels.rows;i++)
    {
        forward_mapper[labels.at<int>(i)] += 1;
        //std::cout<<"setting "<<labels.at<int>(i)<<" to "<<forward_mapper[labels.at<int>(i)]<<" from "<<_labels_i.at<float>(i, 0)<<"\n";
    }

    for(auto it = forward_mapper.begin(); it != forward_mapper.end(); ++it)
    {
        forward_mapper[it->first] = ii;
        labels_o.push_back(it->first);
        labels_n.push_back(ii);
        ii += 1;
    }
    return true;
}
#endif  // TEST_CODE_FOR_MODEL_LOG_REG
BOOST_AUTO_TEST_CASE(empty_data_set_test_lr)
{
    mlmodels::regression_LR_model model;
    auto m = model.train(mlmodels::training_data{}, 
            mlmodels::class_data{},
            mlmodels::logistic_regression::args{}
        );
    BOOST_TEST((m.get() == nullptr), "the model that we should get back from empty data set should be empty as well");
    mlmodels::testing_result out;
    auto r = model.predict(m, mlmodels::testing_data{}, out);
    BOOST_TEST(r == false, "should failed because training did not run");
    BOOST_TEST(out.empty(), "there should not be any data from the test");
}

BOOST_AUTO_TEST_CASE(test_model_create_lr)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
#ifdef TEST_CODE_FOR_MODEL_LOG_REG
        simulate(mlmodels::detail::prepare_labels(classes, CV_32F));
#else
    try {
        mlmodels::regression_LR_model lg;
        auto m = lg.train(samples,
                classes, 
                mlmodels::logistic_regression::args{}
        );
        BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
        const auto& test_samples = ut_data::samples4test();
        BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
        BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
        auto p = lg.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
        BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
        BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
#ifdef PRINT_MODEL_RESULTS
        for (auto f : p) {
            std::cout<<"["<<f<<"]";
        }
        std::cout<<std::endl;
#endif
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we do not except an exception but we've got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unknown exception while running unit test");
    }
#endif  // TEST_CODE_FOR_MODEL_LOG_REG
}

