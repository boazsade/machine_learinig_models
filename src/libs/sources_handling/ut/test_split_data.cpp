#define BOOST_TEST_MODULE TEST_SOURCE_HANDLING
#include <boost/test/unit_test.hpp>
#include "ut_details/disable_log.h"
#include "interp_tests.h"
#include "helper_operations.hpp"
#include "libs/sources_handling/source_data.h"

namespace
{
    const unsigned int missing[] = {
        1, 4, 6, 10
    };
}   // end of local namespace


BOOST_AUTO_TEST_CASE(test_parsing_scheme_simple) 
{
    do_disable_log();
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(size(data) > 0u);
    auto sliced = slice(data, std::begin(missing)+1, 1); // we need to ignore value 4
    BOOST_TEST_REQUIRE(sliced.size() == 2u);
    BOOST_TEST_REQUIRE(sliced[0].row == 3u);
    BOOST_TEST_REQUIRE(sliced[1].row == 5u);
    BOOST_TEST_REQUIRE(sliced[0].value == values[3]);
    BOOST_TEST_REQUIRE(sliced[1].value == values[5]);
}

BOOST_AUTO_TEST_CASE(test_parsing_scheme_skip) 
{
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(size(data) > 0u);
    const unsigned int missing_local[] = {
        1, 4, 5, 6, 10
    };
    auto sliced = slice(data, std::begin(missing_local)+1, 1); // we need to ignore value 4, but we would skip as a result on 5 and 6 as well
    BOOST_TEST_REQUIRE(sliced.size() == 2u);
    BOOST_TEST_REQUIRE(sliced[0].row == 3u);
    BOOST_TEST_REQUIRE(sliced[1].row == 7u);
    BOOST_TEST_REQUIRE(sliced[0].value == values[3]);
    BOOST_TEST_REQUIRE(sliced[1].value == values[7]);
}

BOOST_AUTO_TEST_CASE(test_parsing_scheme_skip_begin) 
{
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(size(data) > 0u);
    const unsigned int missing_local[] = {
        1, 3, 4, 6, 10
    };
    auto sliced = slice(data, std::begin(missing_local)+1, 1); // we need to ignore value 4, but we would skip as a result on 3
    BOOST_TEST_REQUIRE(sliced.size() == 2u);
    BOOST_TEST_REQUIRE(sliced[0].row == 2u);
    BOOST_TEST_REQUIRE(sliced[1].row == 5u);
    BOOST_TEST_REQUIRE(sliced[0].value == values[2]);
    BOOST_TEST_REQUIRE(sliced[1].value == values[5]);
}

BOOST_AUTO_TEST_CASE(test_parsing_scheme_skip_some) 
{
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(size(data) > 0u);
    auto sliced = slice(data, std::begin(missing)+2, 3); // we need to ignore value 6, and we need to ignore some values along the way
    BOOST_TEST_REQUIRE(sliced.size() == 6u);
//    for (auto v : sliced) {
//        std::cout<<"["<<v.row<<", "<<v.value<<"]";
//    }
//    std::cout<<std::endl;
    BOOST_TEST_REQUIRE(sliced[0].row == 2u);
    BOOST_TEST_REQUIRE(sliced[1].row == 3u);
    BOOST_TEST_REQUIRE(sliced[2].row == 5u);
    BOOST_TEST_REQUIRE(sliced[3].row == 7u);
    BOOST_TEST_REQUIRE(sliced[4].row == 8u);
    BOOST_TEST_REQUIRE(sliced[5].row == 9u);
    BOOST_TEST_REQUIRE(sliced[0].value == values[2]);
    BOOST_TEST_REQUIRE(sliced[1].value == values[3]);
    BOOST_TEST_REQUIRE(sliced[2].value == values[5]);
    BOOST_TEST_REQUIRE(sliced[3].value == values[7]);
    BOOST_TEST_REQUIRE(sliced[4].value == values[8]);
    BOOST_TEST_REQUIRE(sliced[5].value == values[9]);
}

BOOST_AUTO_TEST_CASE(test_transfrom_slice)
{
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(size(data) > 0u);
    auto sliced = slice(data, std::begin(missing)+2, 3); // we need to ignore value 6, and we need to ignore some values along the way
    BOOST_TEST_REQUIRE(sliced.size() == 6u);
    // now check that we extracting the data from this range
    auto v = transform_slice(std::make_pair(std::begin(sliced), std::end(sliced)));
    BOOST_TEST_REQUIRE(v.size() == sliced.size());
    BOOST_TEST_REQUIRE(v[0] == values[2]);
    BOOST_TEST_REQUIRE(v[1] == values[3]);
    BOOST_TEST_REQUIRE(v[2] == values[5]);
    BOOST_TEST_REQUIRE(v[3] == values[7]);
    BOOST_TEST_REQUIRE(v[4] == values[8]);
    BOOST_TEST_REQUIRE(v[5] == values[9]);
    auto r = row_nums_transform(std::make_pair(std::begin(sliced), std::end(sliced)));
    BOOST_TEST_REQUIRE(r.size() == sliced.size());
    BOOST_TEST_REQUIRE(r[0] == 2);
    BOOST_TEST_REQUIRE(r[1] == 3);
    BOOST_TEST_REQUIRE(r[2] == 5);
    BOOST_TEST_REQUIRE(r[3] == 7);
    BOOST_TEST_REQUIRE(r[4] == 8);
    BOOST_TEST_REQUIRE(r[5] == 9);
}

