#ifndef DUMP_ROW_MODEL_OUTPUT
#include <boost/test/unit_test.hpp>
#include "apps/run_experiments/process_input.h"
#include "libs/system/exceptions.h"
#include "apps/run_experiments/processing_pipeline.h"
#include <string>


BOOST_AUTO_TEST_CASE(test_invalid_message)
{
    try {
        process_input(std::string{});
        BOOST_TEST(false, "there should be expcetion here");
    } catch (const invalid_conf&) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting other expcetions");
    }

    try {
        process_input(std::string{"file not found"});
        BOOST_TEST(false, "exception is expected here");
    } catch (const std::exception&) {
    } catch (...) {
        BOOST_TEST(false, "expecting invalid_conf expcetion");
    }

    try {
        process_input(std::string{"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
});
        BOOST_TEST(false, "this is an invalid json input");
    } catch (const std::exception& e) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting to have json error exception");
    }

    
}

BOOST_AUTO_TEST_CASE(test_invalid_message_path)
{
    const std::string invalid_msg{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"pah\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 1},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        process_input(invalid_msg);
        BOOST_TEST(false, "we are expecting to have an exception");
    } catch (const json_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we are expecting to have json expcetion, got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are expecting to have json error expcetion");
    }
    
}

BOOST_AUTO_TEST_CASE(test_invalid_message_model)
{
    const std::string invalid_msg2{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"modell\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 1},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        process_input(invalid_msg2);
        BOOST_TEST(false, "we are expecting to have an exception");
    } catch (const json_error&) {
        BOOST_TEST(false, "we are expecting to have json parsing error");
    } catch (const std::exception& ) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting to have json error expcetion");
    }
    
}
BOOST_AUTO_TEST_CASE(test_invalid_message_invalid_algo)
{
    const std::string invalid_msg3{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 1},\"actions\": [{\"action\": \"model.SVSM\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\", \"Logisic-Los\", \"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        process_input(invalid_msg3);
        BOOST_TEST(false, "we are expecting to have an exception");
    } catch (const invalid_conf& ic) {
    } catch (const json_error&) {
        BOOST_TEST(false, "we expecting invalid config expcetion");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we are expecting to have json expcetion, got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are expecting to have json error expcetion");
    }
}

BOOST_AUTO_TEST_CASE(test_valid_msg)
{
    const std::string msg1{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };

    try {
        process_input(msg1);
        BOOST_TEST(false, "we dont have the correct attributes as values to the SVM algorithm");
    } catch (const json_error&) {
        BOOST_TEST(true, "missing valid attributes");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we are expecting to have json error exception, not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "this should be json exception got other expcetion");
    }
    const std::string msg2{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"termination-count\": 111,\"termination-epsilon\": 2343}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };
    try {
        process_input(msg2);
        BOOST_TEST(false, "there are missing values here, so there should be an exception");
    } catch (const json_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we expecting  to have other expcetion not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unknown excetion, expecting json error");
    }

    const std::string msg3{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"termination-count\": 111,\"termination-epsilon\": 2343, \"kernel\": \"one_class\", \"type\": \"eps_svr\"}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
    };

    try {
        process_input(msg3);
        BOOST_TEST(false, "there should be an exception, invalid kerenl type");
    } catch (const json_error& je) {
        BOOST_TEST(false, "should not be json error, "<<je.what());
    } catch (const std::runtime_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "should be runtime expcetion and not "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are expcetion to have runtime error");
    }
    const std::string msg{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/normalized.csv\"},\"model\": {\"class\": \"mycolumn\",\"type\": \"classifier\",\"repeations\": 43},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"kernel\": \"linear\", \"type\": \"eps_svr\"}}],\"evaluation\": [\"auc-roc\",\"ks-test\"],\"validation\": {\"action\": \"k-fold\",\"attr\": {\"arg\": 10}}}"
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
