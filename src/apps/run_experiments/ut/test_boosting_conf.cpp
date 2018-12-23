#ifndef DUMP_ROW_MODEL_OUTPUT
#include <boost/test/unit_test.hpp>
#include "apps/run_experiments/process_input.h"
#include "libs/system/exceptions.h"
#include "apps/run_experiments/processing_pipeline.h"
#include <string>

BOOST_AUTO_TEST_CASE(invalid_args_boosting)
{
    const std::string invalid_args{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.boosting\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };

    try {
        process_input(invalid_args);
        BOOST_TEST(false, "we dont have the correct attributes as values to the SVM algorithm");
    } catch (const json_error&) {
        BOOST_TEST(true, "missing valid attributes");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we are expecting to have json error exception, not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "this should be json exception got other expcetion");
    }
}

BOOST_AUTO_TEST_CASE(wrong_boosting_args_test)
{
    const std::string args4svm{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"repeations\": 43},\"type\": \"classifier\",\"actions\": [{\"action\": \"model.boosting\",\"attr\": {\"termination-count\": 111,\"termination-epsilon\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        process_input(args4svm);
        BOOST_TEST(false, "the args are for SVM, not EM, so there should be an exception");
    } catch (const json_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we expecting  to have other expcetion not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unknown excetion, expecting json error");
    }
}

BOOST_AUTO_TEST_CASE(missing_boosting_args)
{

    const std::string missing_args{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.boosting\",\"attr\": {\"weak-count\": 111,\"type\": \"type\", \"weight-trim-rate\": 111.55}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };

    try {
        process_input(missing_args);
        BOOST_TEST(false, "there should be an exception, invalid kerenl type");
    } catch (const json_error& ) {
    } catch (const std::runtime_error& e) {
        BOOST_TEST(false, "should not be rumtime error, "<<e.what());
    } catch (const std::exception& e) {
        BOOST_TEST(false, "should be json expcetion and not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are expcetion to have runtime error");
    }
}
BOOST_AUTO_TEST_CASE(valid_boosting)
{
    const std::string msg{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.boosting\",\"attr\": {\"weak-count\": 111,\"type\": \"real\", \"weight-trim-rate\": 1.1,\"max-categories\": 88,\"cv-fold\": 234,\"max-depth\":523,\"min-samples\":98,\"use-surrogates\":true,\"flags\": \"max\"}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        auto conf = process_input(msg);
        BOOST_TEST(pipeline::good(conf), "this is a good configuration so we expecting to have valid results");
        BOOST_TEST(pipeline::source_path(conf).string() == "/home/foo/inputs/normalized.csv");
        BOOST_TEST(pipeline::iterations(conf)== 43u);
        BOOST_TEST(pipeline::models_count(conf) == 1u);
        BOOST_TEST(pipeline::experiment2run(conf) == 2u);
    } catch (const std::exception& e) {
        BOOST_TEST(false, "got exception "<<e.what());
    } catch (const invalid_conf& ic) {
        BOOST_TEST(false, "got invalid configuration "<<ic.what());
    } catch (const json_error& je) {
        BOOST_TEST(false, "JSON error "<<je.what());
    } catch (...) {
        BOOST_TEST(false, "we don't expect to get expcetion here");
    }
}
#endif  // DUMP_ROW_MODEL_OUTPUT
