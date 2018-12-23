#include <boost/test/auto_unit_test.hpp>
#include "inputs/outliers.h"
#include "inputs/data_for_outliers_ut.h"
#include "libs/sources_handling/input_data.h"
#include "apps/detect_duplicates/outliers_action.h"
#include "apps/detect_duplicates/duplication_task.h"
#include "utils/algorithms.hpp"
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

using test_data_outliers = std::vector<input_data>;


template<typename Range>
row_numbers run_detection(policy_action action, const Range& input)
{
    outliers_detector detect_action{action};
    std::for_each(std::begin(input), std::end(input),
                        [&detect_action](auto& data) {
                                ::apply_action(detect_action, data);
                    }
    );
    row_numbers outliers, missings;
    finding(detect_action, outliers, missings);
    return outliers;
}


BOOST_AUTO_TEST_CASE(test_no_outliers)
{
    test_data_outliers not_outliers {
        {1.0, 1}, {1.0, 2}, {1.0, 3}, {1.0, 4},
        {1.0, 5}, {1.0, 6}, {1.0, 7}, {1.0, 8},
    }; // all same, so no outliers
    // build the action that we would apply on this
    row_numbers empty_list; 
    auto outliers = run_detection(outlier_predicate {std::begin(empty_list), std::end(empty_list)}, 
            not_outliers);
     
    BOOST_TEST(outliers.empty(), "we are not expecting to find any outliers in this input");
}

BOOST_AUTO_TEST_CASE (test_outiers_no_drops)
{
    test_data_outliers input_data;
    auto index = 1u;
    boost::transform(Input_vector, std::back_inserter(input_data), [&index](auto val) {
                                return test_data_outliers::value_type{val, index++};
                            }
                    );
    row_numbers empty_list; 
    //boost::copy(input_data, std::ostream_iterator<test_data_outliers::value_type>(std::cout, "\n"));
    auto outliers = run_detection(outlier_predicate{std::begin(empty_list), std::end(empty_list)}, input_data);
    //boost::copy(outliers, std::ostream_iterator<row_numbers::value_type>(std::cout, "\n"));
    BOOST_TEST_REQUIRE(!outliers.empty(), "we must have outliers in this list");
    BOOST_TEST_REQUIRE(outliers.size() == utils::size(identified_outliers_idx), 
            "the number of outliers must be "<<utils::size(identified_outliers_idx)<<", but we have "<<outliers.size());
   
    for (auto outlier : outliers) {
        auto i = std::find(std::begin(identified_outliers_idx), std::end(identified_outliers_idx), outlier);
        BOOST_TEST((i != std::end(identified_outliers_idx)), 
                    "outlier index "<<outlier<<" for "<<Input_vector[outlier]<<" must be in the list of outliers");
    } 
}

BOOST_AUTO_TEST_CASE (test_outiers_with_drops)
{
    test_data_outliers input_data;
    auto index = 1u;
    boost::transform(Input_vector, std::back_inserter(input_data), [&index](auto val) {
                                return test_data_outliers::value_type{val, index++};
                            }
                    );
    row_numbers drops{identified_outliers_idx[0]}; 
    //boost::copy(input_data, std::ostream_iterator<test_data_outliers::value_type>(std::cout, "\n"));
    auto outliers = run_detection(outlier_predicate{std::begin(drops), std::end(drops)}, input_data);
    //boost::copy(outliers, std::ostream_iterator<row_numbers::value_type>(std::cout, "\n"));
    BOOST_TEST_REQUIRE(!outliers.empty(), "we must have outliers in this list");
    BOOST_TEST_REQUIRE(outliers.size() == (utils::size(identified_outliers_idx) - 1), 
            "the number of outliers must be "<<(utils::size(identified_outliers_idx)-1)<<", but we have "<<outliers.size());
   
    for (auto outlier : outliers) {
        if (outlier != identified_outliers_idx[0]) {
            auto i = std::find(std::begin(identified_outliers_idx), std::end(identified_outliers_idx), outlier);
            BOOST_TEST((i != std::end(identified_outliers_idx)), 
                    "outlier index "<<outlier<<" for "<<Input_vector[outlier]<<" must be in the list of outliers");
        }
    } 
}

BOOST_AUTO_TEST_CASE (test_outiers_with_duplications)
{
    const row_numbers::value_type expected_rows[] = {
        123,  449,  499,  982,  993, 1015, 1016, 1031, 1042, 1043, 1045,
        1046, 1149, 1240, 2015, 2313, 2948
    };
    auto outliers = run_detection(outlier_predicate{}, outliers_input_data);
    BOOST_TEST_REQUIRE(outliers.empty() == false);
    std::set<row_numbers::value_type> dups;
    for (auto v : outliers) {
        auto r = dups.insert(v);
        BOOST_TEST(r.second, 
                "the value "<<v<<" was already in the list of outliers - we should not have duplications!");
    }
    std::cout<<std::endl;
    BOOST_TEST_REQUIRE(utils::size(expected_rows) == outliers.size());
    std::sort(std::begin(outliers), std::end(outliers));
    for (auto i = 0u; i < outliers.size(); ++i) {
        BOOST_TEST(expected_rows[i] == outliers[i], 
                "the value "<<expected_rows[i]<<" was expected to be in outliers list but we have "<<outliers[i]);
    }
}

