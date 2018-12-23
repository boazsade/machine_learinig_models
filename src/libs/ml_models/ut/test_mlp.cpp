#include "test_data.hpp"
#include "../src/shared_operations.hpp"
#include "logging.h"
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/MLP.h"
#include "libs/ml_models/model_types.h"
#include <opencv/cxcore.h>

BOOST_AUTO_TEST_CASE(empty_data_set_test_mlp)
{
    mlmodels::multi_layer_perceptrons  model;
    BOOST_TEST_REQUIRE(!mlmodels::is_classifier<mlmodels::multi_layer_perceptrons>());
    BOOST_TEST_REQUIRE(mlmodels::is_regression<mlmodels::multi_layer_perceptrons>());
    auto m = model.train(mlmodels::training_data{}, 
            mlmodels::class_data{},
            mlmodels::multi_layer_perceptrons::args{}
        );
    BOOST_TEST((m.get() == nullptr), "the model that we should get back from empty data set should be empty as well");
    mlmodels::testing_result out;
    auto r = model.predict(m, mlmodels::testing_data{}, out);
    BOOST_TEST(r == false, "should failed because training did not run");
    BOOST_TEST(out.empty(), "there should not be any data from the test");
}

BOOST_AUTO_TEST_CASE(test_model_create_mlp)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    try {
        mlmodels::multi_layer_perceptrons lg;
        auto m = lg.train(samples,
                classes, 
                mlmodels::multi_layer_perceptrons::args{}
        );
        BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
        const auto& test_samples = ut_data::samples4test();
        BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
        BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
        auto p = lg.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
        BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
        BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we do not except an exception but we've got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unknown exception while running unit test");
    }
}

BOOST_AUTO_TEST_CASE(test_model_create_mlp_args)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    try {
        mlmodels::multi_layer_perceptrons::args a{
            100, 0.0001, 
            mlmodels::multi_layer_perceptrons::args::activation_function{ 1, 1}, 
            mlmodels::multi_layer_perceptrons::args::train_method{ mlmodels::multi_layer_perceptrons::args::BACKPROP, 1, 1},
            mlmodels::multi_layer_perceptrons::layers_t{ 16, 8, 4}
        };
        mlmodels::multi_layer_perceptrons lg;
        auto m = lg.train(samples,
                classes, a
        );
        BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
        const auto& test_samples = ut_data::samples4test();
        BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
        BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
        auto p = lg.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
        BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
        BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
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
}
