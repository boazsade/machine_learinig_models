#include <boost/test/unit_test.hpp>
#include "test_op.hpp"
#include "inputs/minmax.h"
#include "inputs/znorm.h"
#include "libs/model_prediction/prediction_flow.h"
#include "libs/model_prediction/prediction_data.h"
#include "libs/model_prediction/prediction_norm.h"
#include "libs/model_prediction/prediction_status.h"
#include "libs/model_prediction/prediction_norm_task.h"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "utils/algorithms.hpp"
#include "utils/math_datatype.h"

namespace
{

inline math::min_max<double>
    minmax_stats()
{
    return math::min_max_values(min_max_data);
}

minmax_normalization_stats minmax_norm()
{
    static const math::min_max<double> minmax{minmax_stats()};
    return minmax_normalization_stats{0u, 0, 1,
            math::minimum(minmax), math::maximum(minmax)};
}

z_normaliztion_conf_stats zrnom()
{
    return z_normaliztion_conf_stats{1u, PsXY[0], PsXY[1]};
}

}   // end of local namespace

BOOST_AUTO_TEST_CASE(test_no_input_for_norm)
{
    // test what would happen in case we don't have row for norm operation
    prediction::data d{prediction::output{prediction::row_number{1}, prediction::status::OK}};
    const auto& before_action = prediction::current_values(d);
    const auto norm_data{prediction::norm(prediction::normalized{}, d)};
    BOOST_TEST_REQUIRE(empty(norm_data));
    BOOST_TEST(size(norm_data) == 0u);
    BOOST_TEST(state(prediction::current_state(norm_data)) == prediction::status::OK);
    const auto& r = prediction::current_values(norm_data);
    BOOST_TEST_REQUIRE((r == before_action), "we are expecting that the call in this case would do nothing");
}

BOOST_AUTO_TEST_CASE(test_no_action_for_norm)
{
    // in this case we would not have any norm actions, so we 
    // are expecting to have the same data in the input and the output
    // of the operation, but this should not have any issues with the 
    // results of the operation
    const prediction::row::data_type the_data[] = {5.1155f, 67.2244f, 87.234f, 8.83552f, 77.778, 11,442};
    const prediction::row data_before{std::begin(the_data), std::end(the_data), prediction::row_number{16}};
    const auto data_after = prediction::norm(prediction::normalized{}, prediction::data{data_before});
    // we are expecting to not change anything after appllying the operation
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::OK);
    BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) != prediction::status::REMOVED);
    const auto after_action{prediction::current_values(data_after)};
    BOOST_TEST(after_action.size() == utils::size(the_data));
    BOOST_TEST(after_action == data_before);
    BOOST_TEST(not empty(data_after));
}

BOOST_AUTO_TEST_CASE(test_normalized_two_columns)
{
    const prediction::row::data_type column_0_min_max[] = {
        (prediction::row::data_type)min_max_data[0],
        (prediction::row::data_type)min_max_data[1],
        (prediction::row::data_type)min_max_data[2],
        (prediction::row::data_type)min_max_data[3],
        (prediction::row::data_type)min_max_data[4],
        (prediction::row::data_type)min_max_data[5]
    };
    const prediction::row::data_type column_1_znrom[] = {
        (prediction::row::data_type)data_mapstd[0],
        (prediction::row::data_type)data_mapstd[1],
        (prediction::row::data_type)data_mapstd[2],
        (prediction::row::data_type)data_mapstd[3],
        (prediction::row::data_type)data_mapstd[4],
        (prediction::row::data_type)data_mapstd[5]
    };
    prediction::row rows[] = {
        prediction::row{prediction::row_number{1}, 3u},
        prediction::row{prediction::row_number{2}, 3u},
        prediction::row{prediction::row_number{3}, 3u},
        prediction::row{prediction::row_number{4}, 3u},
        prediction::row{prediction::row_number{5}, 3u},
        prediction::row{prediction::row_number{6}, 3u}
    };
    for (unsigned int i = 0u; i < utils::size(column_0_min_max); i++) {
        rows[i][0] = column_0_min_max[i];
        rows[i][1] = column_1_znrom[i];
        rows[i][2] = 11.22f;    // just a number to check for later
    }
    // now we would like to see if normalization works
    prediction::normalized norm_op;
    norm_op.assign(prediction::norm_entry{minmax_norm()});
    norm_op.assign(prediction::norm_entry{zrnom()});
    // normalized each row
    for (unsigned int i = 0u; i < utils::size(rows); ++i) {
        const auto data_after = prediction::norm(norm_op, prediction::data{rows[i]});
        BOOST_TEST(not empty(data_after));
        BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) == prediction::status::OK);
        BOOST_TEST_REQUIRE(state(prediction::current_state(data_after)) != prediction::status::REMOVED);
        const auto after_action{prediction::current_values(data_after)};
        BOOST_TEST(after_action != rows[i]);
        BOOST_TEST(not empty(data_after));
        // check the minmax value
        BOOST_TEST(round_cmp(after_action[0], (prediction::row::data_type)expected_second[i], 0.001f), "value after min max norm "<<after_action[0]<<" != "<<expected_second[i]);
        // check znorm
        BOOST_TEST(round_cmp(after_action[1], (prediction::row::data_type)expected_first_mapstd[i], 0.001f), "value after z norm "<<after_action[0]<<" != "<<expected_first_mapstd[i]);
        // and for the last column make sure that it didn't changed
        BOOST_TEST(after_action[2] == 11.22f);
    }
}

