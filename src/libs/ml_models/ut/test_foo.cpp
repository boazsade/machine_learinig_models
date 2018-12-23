#ifdef DO_NOT_RUN_THIS_TEST
#include "test_data.hpp"
#include "../src/shared_operations.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <opencv2/ml.hpp>
#include <opencv/cxcore.hpp>
BOOST_AUTO_TEST_CASE(foo) 
{
    using namespace cv;
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    // insert code here...
    Ptr<ml::KNearest>  knn(ml::KNearest::create());
    //Mat_<float> trainFeatures(7,4);
    auto trainFeatures{mlmodels::detail::prepare_samples(samples)};
    auto trainLabels{mlmodels::detail::prepare_labels(classes)};
    /*trainFeatures << 0.2,0.2,0.2,0.2,
    0.3,0.3,0.3,0.3,
    0.4,0.4,0.4,0.4,
    0.5,0.5,0.5,0.5,
    0.6,0.6,0.6,0.6,
    1.7,1.7,1.7,1.7,
    1.3,3,2,2;
    
    Mat_<int> trainLabels(1,7);
    trainLabels << 0,1,2,3,4,5,0;*/
    
    knn->train(trainFeatures, ml::ROW_SAMPLE, trainLabels);
    
    const auto& test_samples = ut_data::samples4test();
    auto testFeature{mlmodels::detail::prepare_samples(test_samples)};
    //Mat_<float> testFeature(1,4);
    //testFeature<< 0.2,1.7,1.1,0.3;
    
    
    int K=8;
    Mat response,dist,p;
    knn->findNearest(testFeature, K, p, response, dist);
    std::cout <<"predictions:\n"<< p << std::endl;
    std::cout <<"reponses\n"<< response << std::endl;
    std::cout << "distances:\n"<<dist<< std::endl;
}

BOOST_AUTO_TEST_CASE(foo2)
{
    using namespace cv;
    Ptr<ml::KNearest>  knn(ml::KNearest::create());
    int rows = ut_data::rows_number - ut_data::rows4testing;
    Mat_<float> trainFeatures(rows, 3);
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    //std::copy(std::begin(classes), std::end(classes), std::ostream_iterator<mlmodels::class_data::value_type>(std::cout, ", "));
    //return;
    /*trainFeatures << 0.2,0.2,0.2,0.2,
    0.3,0.3,0.3,0.3,
    0.4,0.4,0.4,0.4,
    0.5,0.5,0.5,0.5,
    0.6,0.6,0.6,0.6,
    1.7,1.7,1.7,1.7,
    1.3,3,2,2;
    */
    Mat_<int> trainLabels(rows, 1);
    
    /*trainLabels << 0,1,2,3,4,5,0;*/
    for (auto i = 0; i < rows; ++i) {
        trainLabels.at<int>(i, 0) = classes[i];
    }
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < 3; j++) {
            trainFeatures.at<float>(i, j) = samples[i][j];
        }
    }
    //std::cout<<"train data:\n"<<trainFeatures<<"\nclasses\n"<<trainLabels<<std::endl;
    knn->train(trainFeatures, ml::ROW_SAMPLE, trainLabels);
    
    const auto& test_samples = ut_data::samples4test();
    //auto testFeature{mlmodels::detail::prepare_samples(test_samples)};
    Mat_<float> testFeature((int)ut_data::rows4testing, 3);
    for (auto i = 0; i < testFeature.rows; i++) {
        for (auto j = 0; j < 3; j++) {
            testFeature.at<float>(i, j) = test_samples[i][j];
        }
    }
    //std::cout<<"test data:\n"<<testFeature<<std::endl;
    //testFeature<< 0.2,1.7,1.1,0.3;
    int K=8;
    Mat response, dist, p;
    knn->findNearest(testFeature, K, p, response, dist);
    std::cout <<"predictions:\n"<< p << std::endl;
    std::cout <<"reponses\n"<< response << std::endl;
    std::cout << "distances:\n"<<dist<< std::endl;
}
#endif
