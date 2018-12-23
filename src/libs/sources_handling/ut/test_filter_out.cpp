#include <boost/test/unit_test.hpp>
#include "helper_operations.hpp"
#include "libs/sources_handling/source_data.h"
#include "interp_tests.h"

BOOST_AUTO_TEST_CASE(test_filter_rows)
{
    auto data = from_double_array(std::begin(values), std::end(values), "foo");
    BOOST_TEST_REQUIRE(utils::size(data) == utils::size(values));
    auto size_before = utils::size(data);
    for (auto missing_row : xi_interp) {
        auto i = find(data, missing_row);
        BOOST_TEST((i != end(data)), "we are expecting to have "<<missing_row<<" inside the old data");
    }
    auto to = filter_out(begin(data), end(data), std::begin(xi_interp), std::end(xi_interp));
    BOOST_TEST_REQUIRE(utils::size(data) == size_before);
    auto new_size = std::distance(begin(data), to);
    BOOST_TEST_REQUIRE(new_size < size_before);
//    BOOST_TEST_REQUIRE(new_size + utils::size(xi_interp) == size_before);
    source_data new_data{data.id(), begin(data), to};
    for (auto missing_row : xi_interp) {
        auto i = find(new_data, missing_row);
        BOOST_TEST((i == end(new_data)), "we are not expecting that row "<<missing_row<<" would be in the dataset after we should have removed it");
    }
}

