#include <boost/test/unit_test.hpp>
#include "letter_mc_fc16_data.h"
#include "sensorless_mc_fc48_data.h"
#include "svmguide_mc_fc10_data.h"
#include "../src/svm_impl/classifiers_generator.h"
#include "../src/svm_impl/svm_model.h"
//#include <chrono>
//#include <iostream>

BOOST_AUTO_TEST_CASE(test_prediction_svm_multiclass_letter_fc16)
{
    // running on train data of 10K samples of 3 features data set
    // data is taken from ./binary_models_files/skin_data
    const auto gamma = 0.5;
    const auto cost = 32;
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
                                        letter_mc_fc16_train_features, letter_mc_fc16_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, letter_mc_fc16_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == letter_mc_fc16_predictions.size());
    auto expted_begin = std::begin(letter_mc_fc16_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}
BOOST_AUTO_TEST_CASE(test_prediction_svm_multiclass_correct_splice60)
{
    // running on small dataset with 14 features
    // data was taken from ./binary_models_files/australian
    const auto gamma = 2;
    const auto cost = 32;
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
                                        sensorless_mc_fc48_train_features, sensorless_mc_fc48_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, sensorless_mc_fc48_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == sensorless_mc_fc48_predictions.size());
    auto expted_begin = std::begin(sensorless_mc_fc48_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}

BOOST_AUTO_TEST_CASE(test_prediction_svm_multiclass_svmguid_fc10)
{
    // running on small dataset with 8 features
    // data was taken from ./binary_models_files/diabetes
    const auto gamma = 0.03125;
    const auto cost = 2048;
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
                                        svmguide_mc_fc10_train_features, svmguide_mc_fc10_labels
                                    );
   BOOST_TEST((model.get() != nullptr), "we should get a valid model here"); 
   auto predictions = svm::predict(model, svmguide_mc_fc10_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == svmguide_mc_fc10_predictions.size());
    auto expted_begin = std::begin(svmguide_mc_fc10_predictions);
    unsigned int at = 0;
    for (auto our_results : predictions) {
        BOOST_TEST(our_results == *expted_begin, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
        ++expted_begin;
        ++at;
    }
}

