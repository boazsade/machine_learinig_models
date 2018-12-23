#include <boost/test/unit_test.hpp>
#include "inputs/interp_tests.h"
#include "utils/interpolations.h"
#include "utils/algorithms.hpp"
#include "libs/sources_handling/source_data.h"
#include "slice4missing.hpp"
#include <vector>

BOOST_AUTO_TEST_CASE(test_linear)
{
    std::vector<double> input_data(std::begin(values), std::end(values));
    auto missing_begin = std::begin(xi_interp);
    auto missing_end = std::end(xi_interp);
    BOOST_TEST_REQUIRE(utils::size(expected_linear_interp) == utils::size(xi_interp));  // make sure input is valid!!
    std::size_t count = 0u;
    while (missing_begin != missing_end) {
        auto expected = expected_linear_interp[count];
        //utils::fx_list_type::value_type x = static_cast<utils::fx_list_type::value_type>(*missing_begin);
        //const utils::fx_list_type xi{x -1, x + 1};
        auto input = slice_data(input_data, missing_begin, 1u);
        //std::cout<<"input data at "<<*missing_begin<<"[";
        //out_range(std::cout, input, " ");
        //std::cout<<"]"<<std::endl;
        auto calc = utils::linear_interpolation(input, xi_values(missing_begin, 1), 
                static_cast<math::fx_list_type::value_type>(*missing_begin));/*, xi, x*/
        if (!round_cmp(expected, calc, 0.0001)) { 
            std::cout<<"input is [";
            out_range(std::cout, input, " ");
            std::cout<<"]";
            BOOST_TEST(false, "- fix value "<<calc<<" != expected "<<expected<<" at "<<*missing_begin);
        }
        ++missing_begin;
        ++count;
    }
}

BOOST_AUTO_TEST_CASE(test_linear_with_gaps)
{
    auto dataset = from_double_array(std::begin(values), std::end(values), "rand_name");
    // this unit test is more like the way we run in the system - we have a list of 
    // all the values and the row numbers and we are extracting the valid values with the
    // row numbers one at the time and verifing that it is correct
    auto missing_begin = std::begin(xi_interp);
    auto missing_end = std::end(xi_interp);
    BOOST_TEST_REQUIRE(utils::size(expected_linear_interp) == utils::size(xi_interp));  // make sure input is valid!!
    unsigned int count = 0u;
    while (missing_begin != missing_end) {
        auto expected = expected_linear_interp[count];
        auto subset = slice(dataset, missing_begin, 1u);    // the the subset that we need to fix
        BOOST_TEST_REQUIRE(utils::size(subset) == 2u);        // win size * 2
        BOOST_TEST_REQUIRE(subset[0].value != *missing_begin);
        BOOST_TEST_REQUIRE(subset[1].value != *missing_begin);
        const auto vals = transform_slice(std::begin(subset), std::end(subset));
        const auto cords = row_nums_transform(std::begin(subset), std::end(subset));
        BOOST_TEST_REQUIRE(vals.size() == cords.size());
        BOOST_TEST_REQUIRE(vals.size() == subset.size());
        auto calc = utils::linear_interpolation(vals, cords, 
                static_cast<math::fx_list_type::value_type>(*missing_begin));/*, xi, x*/
        if (!round_cmp(expected, calc, 0.0001)) { 
            std::cout<<"input is [";
            out_range(std::cout, vals, " ");
            std::cout<<"]";
            BOOST_TEST(false, "- fix value "<<calc<<" != expected "<<expected<<" at "<<*missing_begin);
        }
        ++missing_begin;
        ++count;

    }
}
