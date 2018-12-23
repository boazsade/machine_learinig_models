#include <boost/test/unit_test.hpp>
#include "cadata_fc8_data.h"
#include "space_ga_fc6_data.h"
#include "../src/svm_impl/svm_model.h"
#include "../src/svm_impl/regressions_generator.h"
#include "utils/algorithms.hpp"

BOOST_AUTO_TEST_CASE(test_prediction_svm_regression_space)
{
    // for these models we would be using default parameters and 
    // the default kernel and type
    using namespace mlmodels;

    auto model = svm::regression::create_model(svm::regression::nu_rbf_train{
                                            svm::regression::nu_rbf_train::base_1 {
                                                    svm::rbf_args{},
                                                    svm::base_train_params{
                                                        10, 1e-4, true, false
                                                    }},
                                            svm::regression::nu_rbf_train::base_2{}
                                        },
                                        space_ga_fc6_train_features, space_ga_fc6_labels
                                    );
    BOOST_TEST_REQUIRE((model.get() != nullptr), "this should not fail - these are valid paramters to train a model");
   auto predictions = svm::predict(model, space_ga_fc6_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == space_ga_fc6_predictions.size());
    auto expted_begin = std::begin(space_ga_fc6_predictions);
    unsigned int at = 0;
    auto count = 0;
    for (auto our_results : predictions) {
        if (not round_cmp(our_results, static_cast<value_type>(*expted_begin), 0.01f)) {
            BOOST_TEST(false, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin);
            ++count;
        }
        ++expted_begin;
        ++at;
    }
    BOOST_TEST(count == 0, "we have some mismatches between our results and expeected results");
}

BOOST_AUTO_TEST_CASE(test_prediction_svm_regression_cadata)
{
    // for these models we would be using default parameters and 
    // the default kernel and type
    using namespace mlmodels;

    auto model = svm::regression::create_model(svm::regression::nu_rbf_train{
                                            svm::regression::nu_rbf_train::base_1 {
                                                    svm::rbf_args{},
                                                    svm::base_train_params{
                                                        10, 1e-4, true, false
                                                    }},
                                                svm::regression::nu_rbf_train::base_2{}
                                        },
                                        cadata_fc8_train_features, cadata_fc8_labels
                                    );
    BOOST_TEST_REQUIRE((model.get() != nullptr), "this should not fail - these are valid paramters to train a model");
   auto predictions = svm::predict(model, cadata_fc8_test_features);
   BOOST_TEST_REQUIRE(not predictions.empty(), "we should be getting a valid list of prediction here");
   BOOST_TEST_REQUIRE(predictions.size() == cadata_fc8_predictions.size());
    auto expted_begin = std::begin(cadata_fc8_predictions);
    unsigned int at = 0;
    auto count = 0;
    for (auto our_results : predictions) {
        if (static_cast<int>(our_results - *expted_begin) != 0) {
            BOOST_TEST(false, "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin 
                    <<" delta: "<< static_cast<int>(our_results - *expted_begin));
            ++count;
        }
        ++expted_begin;
        ++at;
    }
    BOOST_TEST(count == 0, "we have some mismatches between our results and expeected results");
}



