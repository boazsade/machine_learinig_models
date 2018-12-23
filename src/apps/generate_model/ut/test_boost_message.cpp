#define BOOST_TEST_MODULE TEST_MODEL_GENERATE
#include "message_boosting.h"
#include "ut_details/disable_log.h"
#include "apps/generate_model/process_input.h"
#include "libs/system/exceptions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_invalid_message)
{
    do_disable_log();

    try {
       process_input(std::string());
        BOOST_TEST(false, "we are expecting to get an exception in the case of empty input"); 
    } catch (const invalid_conf&) {
    } catch (...) {
        BOOST_TEST(false, "the exception should be invalid conf");
    }
    try {
        process_input(std::string("this is invalid path"));
        BOOST_TEST(false, "we are expecting to get invalid conf exception");
    } catch (const std::exception&) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting to get invalid conf exception");
    }
}

BOOST_AUTO_TEST_CASE(test_input_message) 
{
    try {
        const auto conf = process_input(BOOSTING_MESSAGE);
        BOOST_TEST(good(conf));
        BOOST_TEST(iterations(conf) == 1u);
        BOOST_TEST(models_count(conf) == 1u);
        BOOST_TEST(source_path(conf).string()  == "/home/boaz/work/machine_learinig_models/src/apps/run_experiments/ut/input.csv");
    } catch (const invalid_conf& e) {
        BOOST_TEST(false, "we should not have failed but got "<<e.what());
    } catch (const std::exception& e) {
        BOOST_TEST(false, "got standard exception "<<e.what());
    }
}
