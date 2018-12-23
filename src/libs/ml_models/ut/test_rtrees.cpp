
#include "test_data.hpp"
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/random_trees.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(empty_data_rtree)
{
    mlmodels::random_trees model;
    auto m = model.train(mlmodels::training_data{}, 
            mlmodels::class_data{},
            mlmodels::random_trees::args{}
        );
    BOOST_TEST((m.get() == nullptr), "the model that we should get back from empty data set should be empty as well");
    mlmodels::testing_result out;
    auto r = model.predict(m, mlmodels::testing_data{}, out);
    BOOST_TEST(r == false, "should failed because training did not run");
    BOOST_TEST(out.empty(), "there should not be any data from the test");
}

BOOST_AUTO_TEST_CASE(test_model_create_rtree)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    
    mlmodels::random_trees b;
    auto m = b.train(samples,
                classes, 
                mlmodels::random_trees::args{}
        );
    BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
    const auto& test_samples = ut_data::samples4test();
    BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
    BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
    auto p = b.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
    BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
    BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
#ifdef PRINT_MODEL_RESULTS
    std::cout<<"model prediction for random_trees\n";
    for (auto f : p) {
        std::cout<<"["<<f<<"]";
    }
    std::cout<<std::endl;
#endif
}

