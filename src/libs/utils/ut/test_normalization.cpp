#include <boost/test/unit_test.hpp>
#include "utils/normalizations.h"
#include "inputs/minmax.h"
#include "inputs/zmaxnorm.h"
#include "inputs/znorm.h"
#include "inputs/column.h"
#include "utils/algorithms.hpp"
#include <boost/range/algorithm/sort.hpp>

BOOST_AUTO_TEST_CASE(test_znorm)
{
    const std::vector<double> test_data(std::begin(data_mapstd), std::end(data_mapstd));
    auto after_norm{utils::z_normalization(test_data)};
    BOOST_TEST_REQUIRE(after_norm.size() == test_data.size());
    BOOST_TEST_REQUIRE(after_norm.size() == utils::size(expected_first_mapstd));
    std::vector<double> expected(std::begin(expected_first_mapstd), std::end(expected_first_mapstd));
    boost::sort(after_norm);
    boost::sort(expected);
    auto ex_begin = std::begin(expected);
    for (auto n_begin = std::begin(after_norm); n_begin != std::end(after_norm); ++n_begin, ++ex_begin) {
        BOOST_TEST(round_cmp(*n_begin, *ex_begin, 0.0001), "value after the norm "<<*n_begin<<" != expected "<<*ex_begin);
    }

}

BOOST_AUTO_TEST_CASE(test_minmax_norm)
{
    const std::vector<double> test_data(std::begin(min_max_data), std::end(min_max_data));
    double min_val = PsY[0]; 
    double max_val = PsY[1];
    auto after_norm = utils::min_max_normalization(test_data, min_val, max_val);
    auto found = std::none_of(std::begin(after_norm), std::end(after_norm), 
                        [min_val, max_val](auto v)
                        {
                            return v < min_val || v > max_val;
                        }
                    );
    BOOST_TEST_REQUIRE(found, 
            "we don't expect to have any value after norm that is less than "
            <<min_val<<" or larger than "<<max_val);
    BOOST_TEST_REQUIRE(after_norm.size() == utils::size(expected_first), 
            "the size of the norm output "<<after_norm.size()<<" not the same as the size "<<
            utils::size(expected_first)<<" that we are expecting");
    std::vector<double> expected(std::begin(expected_first), std::end(expected_first));
    boost::sort(after_norm);
    boost::sort(expected);
    auto expect_begin = std::begin(expected);
    for (auto norm_begin = std::begin(after_norm); norm_begin != std::end(after_norm); ++norm_begin, ++expect_begin) {
        BOOST_TEST(round_cmp(*norm_begin, *expect_begin, 0.0001), "value for "<<*norm_begin<<" != expected "<<*expect_begin);
    }
}
BOOST_AUTO_TEST_CASE(test_minmax_norm_second)
{
    const std::vector<double> test_data(std::begin(min_max_data), std::end(min_max_data));
    double min_val = PsY2[0]; 
    double max_val = PsY2[1];
    auto after_norm = utils::min_max_normalization(test_data, min_val, max_val);
    auto found = std::none_of(std::begin(after_norm), std::end(after_norm), 
                        [min_val, max_val](auto v)
                        {
                            return v < min_val || v > max_val;
                        }
                    );
    BOOST_TEST_REQUIRE(found, 
            "we don't expect to have any value after norm that is less than "
            <<min_val<<" or larger than "<<max_val);
    BOOST_TEST_REQUIRE(after_norm.size() == utils::size(expected_second), 
            "the size of the norm output "<<after_norm.size()<<" not the same as the size "<<
            utils::size(expected_second)<<" that we are expecting");
    std::vector<double> expected(std::begin(expected_second), std::end(expected_second));
    boost::sort(after_norm);
    boost::sort(expected);
    auto expect_begin = std::begin(expected);
    for (auto norm_begin = std::begin(after_norm); norm_begin != std::end(after_norm); ++norm_begin, ++expect_begin) {
        BOOST_TEST(round_cmp(*norm_begin, *expect_begin, 0.0001), "value for "<<*norm_begin<<" != expected "<<*expect_begin);
    }
}

BOOST_AUTO_TEST_CASE(test_max_norm)
{
    const std::vector<double> test_data(std::begin(data_zmax), std::end(data_zmax));
    auto after_norm{utils::max_normalization(test_data)};
    BOOST_TEST_REQUIRE(after_norm.size() == test_data.size());
    BOOST_TEST_REQUIRE(after_norm.size() == utils::size(expected_zmax));
    std::vector<double> expected(std::begin(expected_zmax), std::end(expected_zmax));
    auto f = std::all_of(std::begin(after_norm), std::end(after_norm), [](auto val) {
                return val >= 0 && val <= 1;
            }
    );
    BOOST_TEST_REQUIRE(f, "we have value that is bigger than 1 or smaller than 0");
    //return;
    boost::sort(after_norm);
    boost::sort(expected);
    auto ex_begin = std::begin(expected);
    for (auto n_begin = std::begin(after_norm); n_begin != std::end(after_norm); ++n_begin, ++ex_begin) {
        BOOST_TEST(round_cmp(*n_begin, *ex_begin, 0.0001), "value after the norm "<<*n_begin<<" != expected "<<*ex_begin);
    }
}
BOOST_AUTO_TEST_CASE(test_nan_issue)
{
    const std::vector<double> test_data(std::begin(column_values), std::end(column_values));
    auto after_norm{utils::min_max_normalization(test_data, 0, 1)};
    BOOST_TEST(std::none_of(std::begin(after_norm), std::end(after_norm), [](auto val) {
                        if (std::isnan(val) || std::isinf(val)) {
                            BOOST_TEST(false, "value "<<val<<" is not valid");
                            return true;
                        }
                        return false;
                    }
            )
    );
}

