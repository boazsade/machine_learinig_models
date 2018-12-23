#define BOOST_TEST_MODULE TEST_MODEL_CLASS
#include "test_data.hpp"
#include "logging.h"
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/EM.h"
#include <iostream>

struct disable_log
{
    disable_log() {
        sl::set_normal_log_file("/dev/null");
        sl::set_special_log_file("/dev/null");
        sl::disable_logging();
    }
};

void do_disable_log()
{
    static const disable_log rr = disable_log{}; 
    (void)rr;
}

BOOST_AUTO_TEST_CASE(empty_data_set_test)
{
    do_disable_log();
    mlmodels::expectation_maximization model;
    auto m = model.train(mlmodels::training_data{}, 
            mlmodels::class_data{},
            mlmodels::expectation_maximization::args{}
        );
    BOOST_TEST((m.get() == nullptr), "the model that we should get back from empty data set should be empty as well");
    mlmodels::testing_result out;
    auto r = model.predict(m, mlmodels::testing_data{}, out);
    BOOST_TEST(r == false, "should failed because training did not run");
    BOOST_TEST(out.empty(), "there should not be any data from the test");
}

BOOST_AUTO_TEST_CASE(test_model_create)
{
    const auto& samples = ut_data::unittest_data();
    const auto& classes = ut_data::classes4ut();
    BOOST_TEST_REQUIRE(mlmodels::rows(samples) == classes.size());
    
    mlmodels::expectation_maximization em_m;
    auto m = em_m.train(samples,
                classes, 
                mlmodels::expectation_maximization::args{}
        );
    BOOST_TEST_REQUIRE((m.get() != nullptr), "we are expecting that this train would succeed");
    const auto& test_samples = ut_data::samples4test();
    BOOST_TEST_REQUIRE(mlmodels::rows(test_samples) < mlmodels::rows(samples));
    BOOST_TEST_REQUIRE(mlmodels::columns(test_samples) == mlmodels::columns(samples));
    auto p = em_m.test(m, test_samples, ut_data::NUMBER_OF_CLASSES);
    BOOST_TEST_REQUIRE(!p.empty(), "we are expecting to get entries from the test");
    BOOST_TEST_REQUIRE(p.size() == mlmodels::rows(test_samples));
    //mlmodels::save_model(m);
#ifdef PRINT_MODEL_RESULTS
    std::cout<<std::endl<<"----------------------------\ndata for train\n--------------------------------"<<std::endl;
    for (auto r : samples) {
        for (auto c : r) {
            std::cout<<c<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"\n------------------data test\n-----------------------"<<std::endl;
    for (auto r : test_samples) {
        for (auto c : r) {
            std::cout<<c<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"\n--------------------\npredictions\n----------------"<<std::endl;
    for (auto f : p) {
        std::cout<<"["<<f<<"]";
    }
    std::cout<<std::endl;
#endif
}

