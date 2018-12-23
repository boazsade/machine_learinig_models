#include <boost/test/unit_test.hpp>
#include "shared_operations.h"
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "utils/algorithms.hpp"
#include "inputs/datainput.h"
#include "inputs/interp_tests.h"
#include "inputs/fill_missing_cubic.h"
#include "inputs/fill_missing_knn.h"
#include "inputs/fill_missing_linear.h"
#include <boost/range/algorithm/equal.hpp>

constexpr unsigned int first_missing_row = 0;
constexpr unsigned int missing_row_step = 200;

BOOST_AUTO_TEST_CASE( test_fix_no_missing )
{
    auto data = make_test_data(20, "some id");
    auto fixed_data = preform_action(fix_nothing, data);
    BOOST_TEST(boost::equal(data, fixed_data), "we are expectig the same data, as this do nothing");
}

BOOST_AUTO_TEST_CASE( test_missing_one_entry )
{
    constexpr source_data::data_type::index_type index = 30;
    constexpr source_data::data_type::value_type invalid_data_entry = 1234.456679;
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "missing_input");
    BOOST_TEST_REQUIRE(size(input_data) == std::distance(std::begin(inputdata), std::end(inputdata)),
                "the size of the data we generated for the test "<<size(input_data)<<" != "<<std::distance(std::begin(inputdata), std::end(inputdata)));
    const fillinvalid_base::row_number row_indices[] = {
                            index
    };
    cubical_fix conf{1, std::begin(row_indices), std::end(row_indices)};
    auto entry = find(input_data, index);
    BOOST_TEST_REQUIRE(entry != end(input_data), "we failed to find index "<<index<<" in the input for the test");
    entry->value = invalid_data_entry;
    auto fixed_data = preform_action(conf, input_data);
    BOOST_TEST_REQUIRE(size(input_data) == size(fixed_data), "we expecting to have same size "<<size(input_data)<<" and after "<<size(fixed_data));
    BOOST_TEST(boost::equal(input_data, fixed_data) == false,
            "we are expecting that the fixed data is not the same as the input data, as we changed it");
    entry = find(fixed_data, index);
    BOOST_TEST_REQUIRE(entry != end(fixed_data), "we failed to find index "<<index<<" in the input for the test");
    BOOST_TEST(entry->value != invalid_data_entry, "we expecting that in the fix data we would not have "<<invalid_data_entry);
}

BOOST_AUTO_TEST_CASE( test_missing_first_cubic )
{
    constexpr source_data::data_type::value_type invalid_data_entry = 1234.456679;
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "missing_input");
    const fillinvalid_base::row_number row_indices[] = {
                        missing_row_step
        };

    auto it = std::find_if(std::begin(input_data), std::end(input_data), [](auto entry) {
                            return entry.row == missing_row_step;
                    }
                );
    BOOST_TEST_REQUIRE((it != std::end(input_data)));
    it->value = invalid_data_entry;
    cubical_fix conf{1, std::begin(row_indices), std::end(row_indices)};
    //auto fixed_value = cubic_missing[1];
    auto fixed_data = preform_action(conf, input_data);
    it = std::find_if(std::begin(fixed_data), std::end(fixed_data), [](auto entry) {
                                return entry.row == missing_row_step;
                        }
        );
    BOOST_TEST(it->value != invalid_data_entry, 
            "the value returned for fixing invalid data at "<<*it<<" should not be "<<invalid_data_entry<<" but is "<<it->value);
}

BOOST_AUTO_TEST_CASE( test_missing_first_linear )
{
    constexpr source_data::data_type::value_type invalid_data_entry = 1234.456679;
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "missing_input");
    const fillinvalid_base::row_number row_indices[] = {
                        missing_row_step
        };

    auto it = std::find_if(std::begin(input_data), std::end(input_data), [](auto entry) {
                            return entry.row == missing_row_step;
                    }
                );
    BOOST_TEST_REQUIRE((it != std::end(input_data)));
    it->value = invalid_data_entry;
    linear_fix conf{1, std::begin(row_indices), std::end(row_indices)};
    //auto fixed_value = cubic_missing[1];
    auto fixed_data = preform_action(conf, input_data);
    it = std::find_if(std::begin(fixed_data), std::end(fixed_data), [](auto entry) {
                                return entry.row == missing_row_step;
                        }
        );
    BOOST_TEST(it->value != invalid_data_entry, 
            "the value returned for fixing invalid data at "<<*it<<" should not be "<<invalid_data_entry<<" but is "<<it->value);
}

BOOST_AUTO_TEST_CASE( test_missing_first_knn )
{
    constexpr source_data::data_type::value_type invalid_data_entry = 1234.456679;
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "missing_input");
    const fillinvalid_base::row_number row_indices[] = {
                        missing_row_step
        };

    auto it = std::find_if(std::begin(input_data), std::end(input_data), [](auto entry) {
                            return entry.row == missing_row_step;
                    }
                );
    BOOST_TEST_REQUIRE((it != std::end(input_data)));
    it->value = invalid_data_entry;
    kneighborhood_fix conf{1, std::begin(row_indices), std::end(row_indices)};
    //auto fixed_value = cubic_missing[1];
    auto fixed_data = preform_action(conf, input_data);
    it = std::find_if(std::begin(fixed_data), std::end(fixed_data), [](auto entry) {
                                return entry.row == missing_row_step;
                        }
        );
    BOOST_TEST(it->value != invalid_data_entry, 
            "the value returned for fixing invalid data at "<<*it<<" should not be "<<invalid_data_entry<<" but is "<<it->value);
}

BOOST_AUTO_TEST_CASE(test_fix_missing_linear)
{
    auto input_data = from_double_array(std::begin(values), std::end(values), "missing_input");
    linear_fix conf{1, std::begin(xi_interp), std::end(xi_interp)};
    auto fixed_data = preform_action(conf, input_data); 
    // after we run the fix operation - we have the location where this took place, so now
    // we can comapre this with the expected values for the fixes
    BOOST_TEST_REQUIRE(size(fixed_data) == utils::size(xd_interp));
    BOOST_TEST_REQUIRE(size(fixed_data) >= utils::size(xi_interp));
    std::size_t count = 0u;
    for (auto index : xi_interp) {
        auto it = find(fixed_data, index);
        BOOST_TEST_REQUIRE((it != end(fixed_data)), "failed to find index "<<index<<" in test data!!");
        BOOST_TEST(round_cmp(it->value, expected_linear_interp[count], 0.0001),
                "the value we fixed "<<it->value<<" != "<<expected_linear_interp[count]<<
                " ("<<count<<", index "<<index<<")");
        ++count;
    }
}

BOOST_AUTO_TEST_CASE(test_fix_missing_cubic)
{
    auto input_data = from_double_array(std::begin(values), std::end(values), "missing_cubic");
    cubical_fix conf{1, std::begin(xi_interp), std::end(xi_interp)};
    auto fixed_data = preform_action(conf, input_data); 
    // after we run the fix operation - we have the location where this took place, so now
    // we can comapre this with the expected values for the fixes
    BOOST_TEST_REQUIRE(size(fixed_data) == utils::size(xd_interp));
    BOOST_TEST_REQUIRE(size(fixed_data) >= utils::size(xi_interp));
    std::size_t count = 0u;
    for (auto index : xi_interp) {
        auto it = find(fixed_data, index);
        BOOST_TEST_REQUIRE((it != end(fixed_data)), "failed to find index "<<index<<" in test data!!");
        BOOST_TEST(round_cmp(it->value, expected_cubic_interp[count], 0.0001),
                "the value we fixed "<<it->value<<" != "<<expected_cubic_interp[count]<<
                " ("<<count<<", index "<<index<<")");
        ++count;
    }
}


