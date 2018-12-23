#define BOOST_TEST_MODULE TEST_MATH_UTILS
#include <boost/test/unit_test.hpp>
#include "utils/algorithms.hpp"
#include "utils/math_utils.h"
#include "utils/math_datatype_io.h"
#include <vector>
#include <sstream>

BOOST_AUTO_TEST_CASE(test_algorithm) 
{
    std::vector<int> empty;
    auto i = binary_find(std::begin(empty), std::end(empty), -1);
    BOOST_TEST((i == std::end(empty)), "this is an empty dataset, there should be nothing that we found there");
    std::vector<int> one{1};
    i = binary_find(std::begin(one), std::end(one), -1);
    BOOST_TEST((i == std::end(one)), "the value should not be found");
    i = binary_find(std::begin(one), std::end(one), 1);
    BOOST_TEST((i != std::end(one)), "we should be able to find the value that is in the list");
    BOOST_TEST((i == std::begin(one)), "the value should be found as the first entry in the list");
    BOOST_TEST((*i == 1), "the value that we found should be the only value in the list");
}

BOOST_AUTO_TEST_CASE(test_mean_std)
{
    const double data[] = {
        -7.1255084970859377E18,
            1.4080084590127016E16,
            4.6402048013512079E18,
            1.7976931348623157,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            1.4080084590127016E16,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            1.4080084590127016E16,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18,
            1.4080084590127016E16,
            -7.1255084970859377E18
    };
    auto result = 
        utils::mean_standard_dev<double>(std::begin(data), std::end(data), utils::size(data));
    BOOST_TEST(std::isinf(math::mean(result)) == 0, "we don't expect this to be inf");
    BOOST_TEST(std::isinf(math::standard_dev(result)) == 0, "we don't expect that the standard dev is inf");
}

