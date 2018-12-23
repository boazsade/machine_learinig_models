#define BOOST_TEST_MODULE TEST_MODEL_TRASFORM_DATA
#include <boost/test/unit_test.hpp>
#include "ut_details/disable_log.h"
#include "libs/transform_action/lookuptable_actions.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/mapvalues_task.h"
#include "libs/transform_action/map_values_conf.h"
#include "libs/transform_action/map_values_op.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/sources_handling/row_data_input.h"
#include <string>

namespace
{
    column_data::data_type first {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
    };
    column_data::data_type second {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200
    };
    column_data::data_type third {
        100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000
    };
    column_data::data_type forth {
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000
    };
    column_data::data_type fith {
        1000000, 2000000, 3000000, 4000000, 50000, 600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000
    };
    column_data::data_type six {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
classifier::read_result test_data() {

    classifier::read_result data{
        {"first", std::begin(first), std::end(first)},
        {"second", std::begin(second), std::end(second)},
        {"third", std::begin(third), std::end(third)},
        {"forth", std::begin(forth), std::end(forth)},
        {"fith", std::begin(fith), std::end(fith)},
        {"six", std::begin(six), std::end(six)}
    };
    return data;
}

}   // end of local namespace

BOOST_AUTO_TEST_CASE(test_null_operation) 
{
    do_disable_log();

    try {
    // create an action that would do nothing
        auto nocreate_table_action = lookup::make_table_get_f(lookup::dont_map_op); 
    //std::cout<<"finish auto nocreate_table_action = lookup::make_table_get_f(lookup::dont_map_op);"<<std::endl;
    // create the no transform action from this and pass the data to it
        const lookup::lookuptable_transform no_transform_action = preform_action(nocreate_table_action, test_data());
    //std::cout<<"finish no_transform_action = preform_action(nocreate_table_action, test_data());"<<std::endl;
        auto not_changed_data = preform_action(no_transform_action, second);    
    //std::cout<<"finish not_changed_data = preform_action(no_transform_action, second);"<<std::endl;
    // now we need to make sure that we do not change the data at all - so 
    // make sure that we have the same data as the input
        BOOST_TEST_REQUIRE(not_changed_data.size() == second.size());
        for (unsigned int i  = 0; i < not_changed_data.size(); ++i) {
            BOOST_TEST(not_changed_data[i] == second[i], "data at "<<i<<" do not match "<<not_changed_data[i]<<" != "<<second[i]);
        }
    } catch (const std::runtime_error& e) {
        BOOST_TEST(false, "error: "<<e.what());
    }
}

BOOST_AUTO_TEST_CASE(test_with_regression_no_transform)
{
    // this should create an action but one that would do nothing down the road,
    // as the column type is not classifier
    try {
        const  lookup::map_value_op table_gen {
                lookup::map_values_conf{"third", actions::REGRESSION_MODEL}
        };
        const lookup::lookuptable_transform no_lookup = preform_action(table_gen, test_data());
        const auto after_no_transform = preform_action(no_lookup, third);
        BOOST_TEST_REQUIRE(after_no_transform.size() == third.size());
        for (unsigned int i  = 0; i < after_no_transform.size(); ++i) {
            BOOST_TEST(after_no_transform[i] == third[i], "data at "<<i<<" do not match "<<after_no_transform[i]<<" != "<<third[i]);
        }
    } catch (const std::runtime_error& e) {
        BOOST_TEST(false, "error: "<<e.what());
    }
}

BOOST_AUTO_TEST_CASE(test_do_transform)
{
    try {
        const  lookup::map_value_op table_gen {
                lookup::map_values_conf{"third", actions::CLASSIFIER_MODEL}
        };
        const lookup::lookuptable_transform transformer{preform_action(table_gen, test_data())};
    
        const auto after_transform = preform_action(transformer, third);
        BOOST_TEST_REQUIRE(after_transform.size() == third.size());
        for (unsigned int i  = 0; i < after_transform.size(); ++i) {
            BOOST_TEST(after_transform[i] != third[i], "data at "<<i<<" do match "<<after_transform[i]<<" != "<<third[i]);
            BOOST_TEST(after_transform[i] == static_cast<float>(i));
        }
    } catch (const std::runtime_error& e) {
        BOOST_TEST(false, "error: "<<e.what());
    }
}

BOOST_AUTO_TEST_CASE(test_transform_table_size) 
{
    try {
        const  lookup::map_value_op table_gen {
                    lookup::map_values_conf{"third", actions::REGRESSION_MODEL}
        };
        const lookup::lookuptable_transform transformer{preform_action(table_gen, test_data())};
        // note that since this is regression model the size of the table must be -1
        const lookup::lookuptable_transform trans{preform_action(table_gen, test_data())};
        auto size = preform_action(trans, lookup::uniqued_op);
        BOOST_TEST_REQUIRE(size == -1);
        // OK - now lets see what would happened when we have classifier model
        const  lookup::map_value_op table_clas {
                    lookup::map_values_conf{"third", actions::CLASSIFIER_MODEL}
        };
        const lookup::lookuptable_transform trans2{preform_action(table_clas, test_data())};
        size = preform_action(trans2, lookup::uniqued_op);
        BOOST_TEST_REQUIRE(size != -1);
        BOOST_TEST_REQUIRE(size == third.size());   // we don't have unique values there..
        // now lets test with a column that have only one value
        const  lookup::map_value_op one_val {
                    lookup::map_values_conf{"six", actions::CLASSIFIER_MODEL}
        };
        const lookup::lookuptable_transform trans_one{preform_action(one_val, test_data())};
        size = preform_action(trans_one, lookup::uniqued_op);
        BOOST_TEST_REQUIRE(size != -1);
        BOOST_TEST_REQUIRE(size == 1);   // we don't have unique values there..
    } catch (const std::runtime_error& e) {
        BOOST_TEST(false, "failed with error "<<e.what());
    }
}

