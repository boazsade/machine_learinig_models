#include <boost/test/unit_test.hpp>
#include "inputs/interp_tests.h"
#include "slice4missing.hpp"
#include "utils/algorithms.hpp"
#include "utils/interpolations.h"

BOOST_AUTO_TEST_CASE(test_knn)
{
    std::vector<double> input_data(std::begin(values), std::end(values));
    auto missing_begin = std::begin(xi_interp);
    auto missing_end = std::end(xi_interp);
    BOOST_TEST_REQUIRE(utils::size(expected_nearest_interp) == utils::size(xi_interp));  // make sure input is valid!!
    std::size_t count = 0u;
    while (missing_begin != missing_end) {
        auto expected = expected_nearest_interp[count];
        auto input = slice_data(input_data, missing_begin, 1u);
        const auto x_cord = xi_values(missing_begin, 1); 
        auto calc = utils::K_neighborhood(input, x_cord,
                static_cast<math::fx_list_type::value_type>(*missing_begin));/*, xi, x*/
        if (!round_cmp(expected, calc, 0.0001)) { 
            std::cout<<"input is [";
            out_range(std::cout, input, " ");
            std::cout<<"] at location [";
            out_range(std::cout, x_cord, " ");
            std::cout<<"] ";
            BOOST_TEST(false, "- fix value "<<calc<<" != expected "<<expected<<" index "<<*missing_begin<<" for value "<<input_data[*missing_begin]);
        }
        ++missing_begin;
        ++count;
    }
}

BOOST_AUTO_TEST_CASE(test_knn_gaps)
{
    // this test is running using the functions that the actual system would use to 
    // extract the data in the format of [value, row number] pair and not just two
    // arrays - one for data and one for the row numbers
    auto dataset = from_double_array(std::begin(values), std::end(values), "rand_name");
    auto missing_begin = std::begin(xi_interp);
    auto missing_end = std::end(xi_interp);
    BOOST_TEST_REQUIRE(utils::size(expected_nearest_interp) == utils::size(xi_interp));  // make sure input is valid!!
    std::size_t count = 0u;
    while (missing_begin != missing_end) {
        auto expected = expected_nearest_interp[count];
        auto subset = slice(dataset, missing_begin, 1u);    // the the subset that we need to fix
        BOOST_TEST_REQUIRE(utils::size(subset) == 2u);        // win size * 2
        const auto vals = transform_slice(std::begin(subset), std::end(subset));
        const auto cords = row_nums_transform(std::begin(subset), std::end(subset));
        BOOST_TEST_REQUIRE(vals.size() == cords.size());
        BOOST_TEST_REQUIRE(vals.size() == subset.size());
        auto calc = utils::K_neighborhood(vals, cords,
                static_cast<math::fx_list_type::value_type>(*missing_begin));/*, xi, x*/
        if (!round_cmp(expected, calc, 0.0001)) { 
            std::cout<<"input is [";
            out_range(std::cout, subset, " ");
            std::cout<<"] at location [";
            out_range(std::cout, cords, " ");
            std::cout<<"] ";
            BOOST_TEST(false, "- fix value "<<calc<<" != expected "<<expected<<" index "<<*missing_begin<<" for value "<<values[*missing_begin]);
        }
        ++missing_begin;
        ++count;
    }
}
