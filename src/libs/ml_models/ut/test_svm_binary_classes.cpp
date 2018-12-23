#include <boost/test/unit_test.hpp>
#include "small_skin_data.h"
#include "splice_fc60_data.h"
#include "sonar_fc60_data.h"
#include "diabetes_fc8_data.h"
#include "../src/svm_impl/classifiers_generator.h"
#include "../src/svm_impl/svm_model.h"
//#include <chrono>
//#include <iostream>

BOOST_AUTO_TEST_CASE(test_prediction_svm_binary_correct_skin_3_features)
{
    // running on train data of 10K samples of 3 features data set
    // data is taken from ./binary_models_files/skin_data
    const auto gamma = 2;
    const auto cost = 512;
    // this test uses the predfined parameters for the testing
    // we are just run train with c_svm and RBF, then compare 
    // the results we have to the ones we collected from libsvm
    using namespace mlmodels;
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{
                                            svm::classifier::c_rbf_train::base_1
                                            {
                                                svm::rbf_args{gamma},
                                                svm::base_train_params{}
                                            },
                                            svm::classifier::c_rbf_train::base_2{
                                                cost, 0, 0
                                            }
                                        },
                                        small_skin_train_features, small_skin_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, small_skin_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == small_skin_predictions.size());
    auto expted_begin = std::begin(small_skin_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}

BOOST_AUTO_TEST_CASE(test_prediction_svm_binary_correct_splice60)
{
    // running on small dataset with 14 features
    // data was taken from ./binary_models_files/australian
    const auto gamma = 0.03125;
    const auto cost = 2;
    // this test uses the predfined parameters for the testing
    // we are just run train with c_svm and RBF, then compare 
    // the results we have to the ones we collected from libsvm
    using namespace mlmodels;
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{
                                            svm::classifier::c_rbf_train::base_1
                                            {
                                                svm::rbf_args{gamma},
                                                svm::base_train_params{}
                                            },
                                            svm::classifier::c_rbf_train::base_2{
                                                cost, 0, 0
                                            }
                                        },
                                        splice_fc60_train_features, splice_fc60_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, splice_fc60_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == splice_fc60_predictions.size());
    auto expted_begin = std::begin(splice_fc60_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}

BOOST_AUTO_TEST_CASE(test_prediction_svm_binary_correct_sonar60)
{
    // running on small dataset with 60 features
    // data was taken from ./binary_models_files/sonar
    const auto gamma = 0.125;
    const auto cost = 2;
    // this test uses the predfined parameters for the testing
    // we are just run train with c_svm and RBF, then compare 
    // the results we have to the ones we collected from libsvm
    using namespace mlmodels;
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{
                                            svm::classifier::c_rbf_train::base_1
                                            {
                                                svm::rbf_args{gamma},
                                                svm::base_train_params{}
                                            },
                                            svm::classifier::c_rbf_train::base_2{
                                                cost, 0, 0
                                            }
                                        },
                                        sonar_fc60_train_features, sonar_fc60_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, sonar_fc60_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == sonar_fc60_predictions.size());
    auto expted_begin = std::begin(sonar_fc60_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}

BOOST_AUTO_TEST_CASE(test_prediction_svm_binary_correct_diabet8)
{
    // running on small dataset with 8 features
    // data was taken from ./binary_models_files/diabetes
    const auto gamma = 0.03125;
    const auto cost = 8;
    // this test uses the predfined parameters for the testing
    // we are just run train with c_svm and RBF, then compare 
    // the results we have to the ones we collected from libsvm
    using namespace mlmodels;
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{
                                            svm::classifier::c_rbf_train::base_1
                                            {
                                                svm::rbf_args{gamma},
                                                svm::base_train_params{}
                                            },
                                            svm::classifier::c_rbf_train::base_2{
                                                cost, 0, 0
                                            }
                                        },
                                        diabetes_fc8_train_features, diabetes_fc8_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, diabetes_fc8_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == diabetes_fc8_predictions.size());
    auto expted_begin = std::begin(diabetes_fc8_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}


