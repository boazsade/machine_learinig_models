#include <boost/test/unit_test.hpp>
#include "svm_test_input.hpp"
#include "../src/svm_impl/classifiers_generator.h"
#include "../src/svm_impl/regressions_generator.h"
#include "../src/svm_impl/regression_params.hpp"
#include "../src/svm_impl/classifier_params.hpp"
#include "../src/svm_impl/svm_model.h"
#include <limits>
#include <stdexcept>
#include <iostream>

namespace 
{

const mlmodels::class_data labels = { 0, 0, 1, 1, 0, 2, 1, 2, 1, 0 };

const mlmodels::training_data dataf = make_multi_array<mlmodels::value_type, 10, 2>(
        { { 100.f, 10.f }, 
          { 150.f, 10.f }, 
          { 600.f, 200.f }, 
          { 600.f, 10.f }, 
          { 10.f, 100.f }, 
          { 455.f, 10.f }, 
          { 345.f, 255.f }, 
          { 10.f, 501.f }, 
          { 401.f, 255.f }, 
          { 30.f, 150.f } 
        }
);

}   // end of namespace

BOOST_AUTO_TEST_CASE(test_invalid_data_for_model)
{
    using namespace mlmodels;
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{}, 
                        training_data{}, class_data{}
            );
    BOOST_TEST((model.get() == nullptr), "we are expecting to fail in model generation as we have invalid data for the model");
    // while it should be the same - test for regression type as well
    model = svm::regression::create_model(svm::regression::epsilon_linear_train{},
                            training_data{}, class_data{}
            );
    BOOST_TEST((model.get() == nullptr), "we are expecting to fail in model generation as we have invalid data for the model");
}

BOOST_AUTO_TEST_CASE(test_train_model)
{
    using namespace mlmodels;
    try {
        auto model = svm::classifier::create_model(svm::classifier::nu_poly_train{}, train_data, train_labels);
        BOOST_TEST((model.get() != nullptr), "we are expecting to successfully pass training with this - if not then we have an anexpected issue here");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "failed to create model: "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unexpted failure in generating the model");
    }

    // now lets try to do the same for regression
    try {
//        std::cout<<"start training on epsilon_rbf_train"<<std::endl;
        auto model = svm::regression::create_model(svm::regression::epsilon_rbf_train{}, dataf, labels);
 //       std::cout<<"finish"<<std::endl;
        BOOST_TEST((model.get() != nullptr), "we are expecting to successfully pass training with this - if not then we have an anexpected issue here");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "failed to create model: "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unexpted failure in generating the model");
    }
}

BOOST_AUTO_TEST_CASE(test_invalid_prediction)
{
    using namespace mlmodels;
    // create invalid model
    auto model = svm::classifier::create_model(svm::classifier::nu_linear_train{}, 
                        training_data{}, class_data{}
            );
    BOOST_TEST_REQUIRE((model.get() == nullptr), "we are expecting to fail in model generation as we have invalid data for the model");
    auto ret = svm::predict(model, labels);
    BOOST_TEST(ret == std::numeric_limits<value_type>::max());
    auto ret2 = svm::test(model, dataf);
    BOOST_TEST(ret2.empty());
    model = svm::regression::create_model(svm::regression::epsilon_sig_train{},
                            training_data{}, class_data{}
            );
    BOOST_TEST((model.get() == nullptr), "we are expecting to fail in model generation as we have invalid data for the model");
    ret = svm::predict(model, labels);
    BOOST_TEST(ret == std::numeric_limits<value_type>::max());
    ret2 = svm::test(model, dataf);
    BOOST_TEST(ret2.empty());
}

BOOST_AUTO_TEST_CASE(test_invalid_model_train_args)
{
    using namespace mlmodels;
    // by passing labels and data with the a different number of elements
    // want to see if this is not accepted
    try {
        auto model = svm::regression::create_model(svm::regression::nu_sig_train{}, dataf, train_labels);
        BOOST_TEST((model.get() == nullptr), "we should get invalid model with these values");
    } catch (const std::exception&) {
        BOOST_TEST(false, "we should get an empty model");
    } catch (...) {
        BOOST_TEST(false, "we should not get a starndard exception, got some other exception");
    }
}

BOOST_AUTO_TEST_CASE(test_invalid_prediction_data_for_model)
{
    using namespace mlmodels;
    // make sure that if we are passing wrong number of features we would fail to predict
    try {
        auto model = svm::classifier::create_model(svm::classifier::c_linear_train{}, train_data, train_labels);
        BOOST_TEST_REQUIRE((model.get() != nullptr), "failed to train the model!!");
        // now try to run prediction and test, and see that it would fail since we are passing the wrong number of features
        auto ret = svm::test(model, dataf);
        BOOST_TEST(ret.empty(), "the args to the model test is wrong - wrong number of features, should return empty"); 
        auto ret2 = svm::predict(model, labels);
        BOOST_TEST((ret2 == std::numeric_limits<value_type>::max()), "the args to predict are wrong, we pass the wrong number of features");
    } catch (...) {
        BOOST_TEST(false, "we should not get any exception here");
    }
}
BOOST_AUTO_TEST_CASE(test_prediction_is_valid_return_value)
{
    using namespace mlmodels;
    // do the same for regression model
    try {
        //auto model = svm::regression::create_model(svm::regression::epsilon_linear_train{}, dataf, labels);
        auto model = svm::classifier::create_model(svm::classifier::c_linear_train{}, dataf, labels);
        BOOST_TEST_REQUIRE((model.get() != nullptr), "failed to train the model!!");
        // pass a row of valid size for prediction
        const class_data row{11.f, 110.f};
        auto ret = svm::test(model, dataf);
        BOOST_TEST(not ret.empty(), "the args to the model test is  - good, should not return empty"); 
        auto ret2 = svm::predict(model, row);
        BOOST_TEST((ret2 != std::numeric_limits<value_type>::max()), "the args to predict are good, we pass good number of features");
    } catch (...) {
        BOOST_TEST(false, "we should not get any exception here");
    }
    return;
    // in this test we would make sure that if we are passing a valid arguments we are getting
    // valid results - by valid it means that they are not out of bound, but we are not checking 
    // if this is a correct prediction mathematically
    try {
        auto model = svm::classifier::create_model(svm::classifier::c_linear_train{}, train_data, train_labels);
        BOOST_TEST_REQUIRE((model.get() != nullptr), "failed to train the model!!");
        // pass a row of valid size for prediction
        const class_data row{0.0416667, -1,-0.333333, -0.283019, -0.260274, 1, 1, 0.343511, 1, -1, -1, -0.333333, -1};
        auto ret = svm::test(model, test_data);
        BOOST_TEST(not ret.empty(), "the args to the model test is  - good, should not return empty"); 
        auto ret2 = svm::predict(model, row);
        BOOST_TEST((ret2 != std::numeric_limits<value_type>::max()), "the args to predict are good, we pass good number of features");
    } catch (...) {
        BOOST_TEST(false, "we should not get any exception here");
    }
    
}

