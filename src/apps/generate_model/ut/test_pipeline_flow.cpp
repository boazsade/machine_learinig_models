#include <boost/test/unit_test.hpp>
#include "message_boosting.h"
#include "message_svm.h"
#include "data_for_testing.h"
#include "columns4testing.h"
#include "data_extract_action.h"
#include "../src/execution_pipline.hpp"
#include "apps/generate_model/process_input.h"
#include "libs/system/exceptions.h"
#include "utils/algorithms.hpp"
#include <algorithm>

BOOST_AUTO_TEST_CASE(test_basic_flow)
{
    try {
        const auto conf = process_input(BOOSTING_MESSAGE);
        const auto lookup_table = conf | classifier::read_result{}; // this should fail to generate a table
        const auto trainer{pipeline::read_step::cast(pipeline::read_step(conf))};   // train step
        const auto ts = trainer | lookup_table;                    // which create the training action
        const auto& data = flowut::data4test();
        const auto train_res = ts | data;
        const auto train_out = train_res | lookup_table | data;      // get the result from the test
        const auto labels = actions::measures(train_out.data());
        const auto ex_act = flowut::make_read_policy(flowut::split_size, flowut::classes_column_id);
        const auto labels_data = preform_action(ex_act.test_reader, data);
        const auto labels_before_test = actions::labels_data(labels_data);
        // and if we would pass this to the next steps it should do nothing
        BOOST_TEST_REQUIRE(labels_before_test.size() == labels.size());
        // we are expecting to have the same data in the labels
        for (auto v : labels_before_test)  {
            bool found_entry = std::any_of(std::begin(labels), std::end(labels), [v] (auto a) {
                                            return a == v;
                                        }
                                    );
            BOOST_TEST(found_entry, "we are expecting to find "<<v<<" in original dataset since we did not change it to be classifier");
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we do not expected to get exceptions here for reading the input but got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we have some expcetion - test failed");
    }
}

BOOST_AUTO_TEST_CASE(test_svm_flow)
{
    (void)INVALID_SVM_MESSAGE;
    try {
        const auto conf = process_input(SVM_MESSAGE);
        const auto lookup_table = conf | classifier::read_result{}; // this should fail to generate a table
        const auto trainer{pipeline::read_step::cast(pipeline::read_step(conf))};   // train step
        const auto ts = trainer | lookup_table;                     // which create the training action
        const auto& data = flowut::data4test();
        const auto train_res = ts | data;
        const auto train_out = train_res | lookup_table | data;     // get the result from the test
        const auto labels = actions::measures(train_out.data());
        const auto ex_act = flowut::make_read_policy(flowut::split_size, flowut::classes_column_id);
        const auto labels_data = preform_action(ex_act.test_reader, data);
        const auto labels_before_test = actions::labels_data(labels_data);
        // and if we would pass this to the next steps it should do nothing
        BOOST_TEST_REQUIRE(labels_before_test.size() == labels.size());
        // we are expecting to have the same data in the labels
        for (auto v : labels_before_test)  {
            bool found_entry = std::any_of(std::begin(labels), std::end(labels), [v] (auto a) {
                                            return a == v;
                                        }
                                    );
            BOOST_TEST(found_entry, "we are expecting to find "<<v<<" in original dataset since we did not change it to be classifier");
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we do not expected to get exceptions here for reading the input but got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we have some expcetion - test failed");
    }
}
BOOST_AUTO_TEST_CASE(test_svm_flow_with_lookup)
{
    (void)INVALID_SVM_MESSAGE;
    try {
        const auto& data = flowut::data4test();
        const auto conf = process_input(SVM_MESSAGE);
        const auto lookup_table = conf | data;                  // generate lookup table 
        const auto trainer{pipeline::read_step::cast(pipeline::read_step(conf))};   // train step
        const auto ts = trainer | lookup_table;                 // which create the training action
        const auto train_res = ts | data;
        const auto train_out = train_res | lookup_table | data; // get the result from the test
        const auto labels = actions::measures(train_out.data());
        const auto ex_act = flowut::make_read_policy(flowut::split_size, flowut::classes_column_id);
        const auto labels_data = preform_action(ex_act.test_reader, data);
        const auto labels_before_test = actions::labels_data(labels_data);
        // and if we would pass this to the next steps it should do nothing
        BOOST_TEST_REQUIRE(labels_before_test.size() == labels.size());
        // we are expecting to not have the same data in the labels
#if 0
        std::cout<<"we have action lookup "<<lookup_table<<"\nlabels_data\n[";
        out_range(std::cout, labels);
        std::cout<<"]\nlabels_before_test:\n";
        out_range(std::cout, labels_before_test);
        std::cout<<std::endl;
#endif
        for (auto v : labels_before_test)  {
            bool found_entry = std::none_of(std::begin(labels), std::end(labels), [v] (auto a) {
                                            return a == v;
                                        }
                                    );
            BOOST_TEST(found_entry, "we are not expecting to find "<<v<<" in original dataset since we change it to be classifier");
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "we do not expected to get exceptions here for reading the input but got "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we have some expcetion - test failed");
    }
}


