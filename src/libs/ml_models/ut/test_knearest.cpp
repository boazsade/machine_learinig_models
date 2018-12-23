
#include "test_data.hpp"
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/K_nearest.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(empty_data_knear_test)
{
    mlmodels::K_nearest model;
    auto m = model.train(mlmodels::training_data{}, 
            mlmodels::class_data{},
            mlmodels::K_nearest::args{}
        );
    BOOST_TEST((m.get() == nullptr), "the model that we should get back from empty data set should be empty as well");
    mlmodels::testing_result out;
    auto r = model.predict(m, mlmodels::testing_data{}, out);
    BOOST_TEST(r == false, "should failed because training did not run");
    BOOST_TEST(out.empty(), "there should not be any data from the test");
}

BOOST_AUTO_TEST_CASE(test_model_create_knear)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    
    mlmodels::K_nearest knear;
    auto m = knear.train(samples,
                classes, 
                mlmodels::K_nearest::args{}
        );
    BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
    const auto& test_samples = ut_data::samples4test();
    BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
    BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
    auto p = knear.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
    BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
    BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
#ifdef PRINT_MODEL_RESULTS
    std::cout<<"model prediction for k nearest\n";
    for (auto f : p) {
        std::cout<<"["<<f<<"]";
    }
    std::cout<<std::endl;
#endif
}
#if 0
BOOST_AUTO_TEST_CASE(test_model_create_knear_to_remove)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    
    mlmodels::K_nearest knear;
    auto m = knear.train(samples,
                    classes, mlmodels::K_nearest::args{}
        );
    BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
    const auto& test_samples = ut_data::samples4test();
    BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
    BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
    //auto p = knear.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
    mlmodels::class_data res, dist, nr;
    BOOST_TEST_REQUIRE(knear.test(m, test_samples, res, dist, nr));
    BOOST_TEST_REQUIRE(!res.empty(), "we are expecting to get entries from the test");
    BOOST_TEST_REQUIRE(res.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
    std::cout<<"model prediction for k nearest\n";
    for (auto f : res) {
        std::cout<<"["<<f<<"]";
    }
    std::cout<<std::endl;
    std::cout<<"distance array of size "<<dist.size()<<std::endl;
    auto i = 0u;
    for (auto d : dist) {
        std::cout<<"["<<d<<"]";
        if ((++i % res.size()) == 0) {
            std::cout<<"\n";
        }
    }
    std::cout<<std::endl;
    std::cout<<"neighbor responses array of size "<<dist.size()<<std::endl;
    i = 0u;
    for (auto r : nr) {
        std::cout<<"["<<r<<"]";
        if ((++i % res.size()) == 0) {
            std::cout<<"\n";
        }
    }
}
#endif
