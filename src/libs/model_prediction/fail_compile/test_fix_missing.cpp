#include <boost/test/unit_test.hpp>
#include "test_op.hpp"
#include "libs/model_prediction/prediction_flow.h"
#include "libs/model_prediction/prediction_data.h"
#include "libs/model_prediction/prediction_row.h"
#include "libs/model_prediction/prediction_output.h"
#include "libs/model_prediction/prediction_status.h"
#include "libs/system/exceptions.h"
#include "libs/sources_handling/map_schema.h"
#include "libs/sources_handling/schema_read.hpp"
#include "utils/algorithms.hpp"
#include "utils/missing_values.hpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


BOOST_AUTO_TEST_CASE(test_no_input_for_fix)
{
    // test what would happen when we are passing empty row - data that has no row
    prediction::data d{prediction::output{prediction::row_number{1}, prediction::status::OK}};
    const auto& before_action = prediction::current_values(d);
    const auto fixed_data{prediction::apply(prediction::verify_fix{}, d)};
    BOOST_TEST_REQUIRE(empty(fixed_data));
    BOOST_TEST(size(fixed_data) == 0u);
    BOOST_TEST(state(prediction::current_state(fixed_data)) == prediction::status::OK);
    const auto& r = prediction::current_values(fixed_data);
    BOOST_TEST_REQUIRE((r == before_action), "we are expecting that the call in this case would do nothing");
}

BOOST_AUTO_TEST_CASE(test_data_no_action_fix)
{
    // in this case we would have good data as input. and not actions to do anything on it
    // we just want to make sure that in this simple case we do nothing
    const prediction::row::data_type the_data[] = {111.555f, 2222.444f, 987.234f, 88833.552f, 4597.001f, 0.45f};
    const prediction::row data_before{std::begin(the_data), std::end(the_data), prediction::row_number{16}};
    const auto data_after = prediction::apply(prediction::verify_fix{}, prediction::data{data_before});
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::OK);
    const auto after_action{prediction::current_values(data_after)};
    BOOST_TEST(after_action.size() == utils::size(the_data));
    BOOST_TEST(after_action == data_before);
}

BOOST_AUTO_TEST_CASE(test_data_with_invalid_no_fix)
{
    // we have a missing value here, but no action to fix it, we want lt see that we are able to report that
    const prediction::row::data_type the_data[] = {111.555f, 2222.444f, 987.234f, 88833.552f, missing_value<prediction::row::data_type>(), 0.45f};
    const prediction::row data_before{std::begin(the_data), std::end(the_data), prediction::row_number{16}};
    const auto data_after = prediction::apply(prediction::verify_fix{}, prediction::data{data_before});
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) != prediction::status::OK);
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::REMOVED);
    const auto after_action{prediction::current_values(data_after)};
    BOOST_TEST(after_action.size() != utils::size(the_data));
    BOOST_TEST(after_action != data_before);
    BOOST_TEST(empty(data_after));
}
BOOST_AUTO_TEST_CASE(test_data_with_invalid_action_fix_not_for_right_row)
{
    const prediction::row::data_type the_data[] = {211.555f, 242.444f, 98.234f, 1833.552f, missing_value<prediction::row::data_type>(), 0.45f};
    const prediction::row data_before{std::begin(the_data), std::end(the_data), prediction::row_number{16}};
    const prediction::fix_missing fix_action{prediction::column_number{2u}, 22.5f}; // "wrong" column number
    prediction::verify_fix fix_op;
    fix_op.assign(fix_action); 
    const auto data_after = prediction::apply(fix_op, prediction::data{data_before});
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) != prediction::status::OK);
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::REMOVED);
    const auto after_action{prediction::current_values(data_after)};
    BOOST_TEST(after_action.size() != utils::size(the_data));
    BOOST_TEST(after_action != data_before);
    BOOST_TEST(empty(data_after));

}

BOOST_AUTO_TEST_CASE(test_data_with_action_fix_on_column)
{
    const prediction::row::data_type the_data[] = {1.1155f, 22.2244f, 987.234f, 88.83552f, missing_value<prediction::row::data_type>(), 1.45f};
    const prediction::row data_before{std::begin(the_data), std::end(the_data), prediction::row_number{16}};
    const prediction::fix_missing fix_action{prediction::column_number{4u}, 22.5f}; // "right" column number
    prediction::verify_fix fix_op;
    fix_op.assign(fix_action); 
    const auto data_after = prediction::apply(fix_op, prediction::data{data_before});
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::OK);
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) != prediction::status::REMOVED);
    const auto after_action{prediction::current_values(data_after)};
    BOOST_TEST(after_action.size() == utils::size(the_data));
    BOOST_TEST(after_action != data_before);
    BOOST_TEST(not empty(data_after));
    BOOST_TEST(missing_value(data_before[4u]));
    BOOST_TEST(not missing_value(after_action[4u]));
    BOOST_TEST(after_action[4u] == 22.5f);
}

