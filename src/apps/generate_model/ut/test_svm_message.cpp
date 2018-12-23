#include "message_svm.h"
#include "ut_details/disable_log.h"
#include "apps/generate_model/process_input.h"
#include "libs/system/exceptions.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_input_message_invalid) 
{
    // the message here of SVM is missing entry - lets check this out
    try {
        process_input(INVALID_SVM_MESSAGE);
        BOOST_TEST(false, "there should be an exception here");
    } catch (const invalid_conf& e) {
        BOOST_TEST(false, "we are expecting to have json error, got "<<e.what());
    } catch (const json_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we are expecting to have json error, got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are expecting to have json error, got unknown error");
    }
}

BOOST_AUTO_TEST_CASE(test_input_message_svm) 
{
    try {
        const auto conf = process_input(SVM_MESSAGE);
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
