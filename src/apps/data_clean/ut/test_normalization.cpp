#include <boost/test/unit_test.hpp>
#include "shared_operations.h"
#include "libs/cleanup_operatiosn/base_normalized_task.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "inputs/normalized_max.h"
#include "inputs/normalized_minmax.h"
#include "inputs/datainput.h"
#include "inputs/normalized_znorm.h"
#include <cmath>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE( test_no_normalization_needed )
{
    // test that you don't do anything when you don't need to do it
    auto input_data = make_test_data(24u, "test input");
    auto ret = preform_action(normalized_nothing, input_data);
    BOOST_TEST(input_data == data(ret), 
            "we do expect that we would not change anything in this case");
}

BOOST_AUTO_TEST_CASE( test_normalized_empty )
{
    normalized_data::arg_type input;
    z_normaliztion_conf znorm;
    auto ret = preform_action(znorm, input);
    const auto& data_after_norm = data(ret);
    BOOST_TEST(data_after_norm == input, "empty data set, no action");
    max_normalization mnorm;
    ret = preform_action(mnorm, input);
    const auto& data_after_norm2 = data(ret);
    BOOST_TEST(data_after_norm2 == input, "we expecting that we do nothing on empty set");
    max_normalization mxnorm;
    ret = preform_action(mxnorm, input);
    const auto& data_after_norm3 = data(ret);
    BOOST_TEST(data_after_norm3 == input, "we expecting that we do nothing on empty set");
    minmax_normalization mmnorm;
    ret = preform_action(mmnorm, input);
    const auto& data_after_norm4 = data(ret);
    BOOST_TEST(data_after_norm4 == input, "we expecting that we do nothing on empty set");
}

BOOST_AUTO_TEST_CASE( test_normalized_max )
{
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "max_normalization");
    auto fixed_data = from_double_array(std::begin(max_normalizated), std::end(max_normalizated), "max normalization");
    max_normalization normalizer{1};
    auto after_fix = preform_action(normalizer, input_data);
    const auto& data_after_norm = data(after_fix);

    BOOST_TEST_REQUIRE(size(input_data) == size(fixed_data), "we expecting that the input data and the data that we fix are the same");
    BOOST_TEST_REQUIRE(size(input_data) == size(data_after_norm), "we are expecting that the data after fix did not changed");
    auto in_be = begin(input_data);
    auto in_end = end(input_data);
    auto exp_be = begin(fixed_data);
    auto exp_end = end(fixed_data);
    for (auto i = begin(data_after_norm); i != end(data_after_norm); ++i) {
        BOOST_TEST_REQUIRE(*i != *in_be, "the fixed data "<<*i<<" do equals "<<*in_be);
        BOOST_TEST_REQUIRE(in_be != in_end, "we should not finish yet!!");
        auto delta_exp = std::fabs(i->value - exp_be->value);
        BOOST_TEST_REQUIRE((delta_exp < 0.001), "the fixed data "<<*i<<" do not equals "<<*exp_be<<" ("<<delta_exp<<")");
        BOOST_TEST_REQUIRE(exp_be != exp_end, "we should not finish yet!!");
        ++in_be;
        ++exp_be;
    }
}

BOOST_AUTO_TEST_CASE( test_normalized_minmax )
{
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "minmax_normalization");
    auto fixed_data = from_double_array(std::begin(minmax_normalized_data), std::end(minmax_normalized_data), "minmax normalization");
    minmax_normalization normalizer{1, -10.0, 10.0};
    auto after_fix = preform_action(normalizer, input_data);
    const auto& data_after_norm = data(after_fix);

    BOOST_TEST_REQUIRE(size(input_data) == size(fixed_data), "we expecting that the input data and the data that we fix are the same");
    BOOST_TEST_REQUIRE(size(input_data) == size(data_after_norm), "we are expecting that the data after fix did not changed");
    auto in_be = begin(input_data);
    auto in_end = end(input_data);
    auto exp_be = begin(fixed_data);
    auto exp_end = end(fixed_data);
    for (auto i = begin(data_after_norm); i != end(data_after_norm); ++i) {
        BOOST_TEST_REQUIRE(*i != *in_be, "the fixed data "<<*i<<" do equals "<<*in_be);
        BOOST_TEST_REQUIRE(in_be != in_end, "we should not finish yet!!");
        auto delta_exp = std::fabs(i->value - exp_be->value);
        BOOST_TEST_REQUIRE((delta_exp < 0.001), "the fixed data "<<*i<<" do not equals "<<*exp_be<<" ("<<delta_exp<<")");
        BOOST_TEST_REQUIRE(exp_be != exp_end, "we should not finish yet!!");
        ++in_be;
        ++exp_be;
    }
}

BOOST_AUTO_TEST_CASE( test_normalized_znorm )
{
    auto input_data = from_double_array(std::begin(inputdata), std::end(inputdata), "z_normalization");
    auto fixed_data = from_double_array(std::begin(znorm_values), std::end(znorm_values), "z normalization");
    z_normaliztion_conf normalizer{1};
    auto after_fix = preform_action(normalizer, input_data);
    const auto& data_after_norm = data(after_fix);

    BOOST_TEST_REQUIRE(size(input_data) == size(fixed_data), "we expecting that the input data and the data that we fix are the same");
    BOOST_TEST_REQUIRE(size(input_data) == size(data_after_norm), "we are expecting that the data after fix did not changed");
    auto in_be = begin(input_data);
    auto in_end = end(input_data);
    auto exp_be = begin(fixed_data);
    auto exp_end = end(fixed_data);
    for (auto i = begin(data_after_norm); i != end(data_after_norm); ++i) {
        BOOST_TEST_REQUIRE(*i != *in_be, "the fixed data "<<*i<<" do equals "<<*in_be);
        BOOST_TEST_REQUIRE(in_be != in_end, "we should not finish yet!!");
        auto delta_exp = std::fabs(i->value - exp_be->value);
        BOOST_TEST_REQUIRE((delta_exp < 0.001), "the fixed data "<<*i<<" do not equals "<<*exp_be<<" ("<<delta_exp<<")");
        BOOST_TEST_REQUIRE(exp_be != exp_end, "we should not finish yet!!");
        ++in_be;
        ++exp_be;
    }
}

