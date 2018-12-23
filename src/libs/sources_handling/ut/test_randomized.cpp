#include <boost/test/unit_test.hpp>
#include "libs/sources_handling/row_data_input.h"
namespace local
{
    column_data::data_type first {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
    };
    column_data::data_type second {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200
    };
    column_data::data_type third {
        100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000
    };
    column_data::data_type forth {
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000
    };
    column_data::data_type fith {
        1000000, 2000000, 3000000, 4000000, 50000, 600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000
    };
classifier::read_result test_data() {

    classifier::read_result data{
        {"first", std::begin(first), std::end(first)},
        {"second", std::begin(second), std::end(second)},
        {"third", std::begin(third), std::end(third)},
        {"forth", std::begin(forth), std::end(forth)},
        {"fith", std::begin(fith), std::end(fith)},
    };
    return data;
}

}   // end of local namespace
namespace mlmodels
{
bool operator == (const training_data& left, const training_data& right) {
   // using index = training_data::index;
    //using row_iterator = training_data::const_iterator;
    //using col_iterator = training_data::subarray<1>::type::const_iterator;

    if (rows(left) == rows(right) && columns(left) == columns(right)) {
        auto r_b = right.begin();
       for (auto l_b = left.begin(); l_b != left.end() && r_b != right.end(); ++r_b, ++l_b) {
           auto c_rb = (*r_b).begin();
           for (auto c_lb = (*l_b).begin(); c_lb != (*l_b).end() && c_rb != (*r_b).end(); ++c_lb, ++c_rb) {
               if (*c_lb != *c_rb) {
                   return false;
               }
           }
       }
       return true; 
    }
    return false;
}
bool operator != (const training_data& left, const training_data& right) {
    return !(left == right);
}

template<typename Row>
bool comp_row(const Row left, const Row right)
{
    auto rr = right.begin();
    for (auto lr = left.begin(); lr != left.end() && rr != right.end(); ++rr, ++lr) {
        if (*lr != *rr) {
            return false;
        }
    }
    return true;
}
// in this case we would like to see that we have the same rows, but we do not
// assume that they are in the same order
bool comp_rows(const training_data& left, const training_data& right) {
    if (rows(left) == rows(right) && columns(left) == columns(right)) {
        for (auto lb = left.begin(); lb != left.end(); ++lb) {
            auto r_b = std::find_if(right.begin(), right.end(), [lb](const auto right_row) {
                        auto c_lb = (*lb).begin();
                        return *c_lb == right_row[0];
                    }
            );
            if (r_b != right.end()) {
                if (!comp_row(*r_b, *lb)) {
                    return false;
                }
            } else {
                return false;
            }
        }
        return true;
    }
    return false;
}

}   // end of namespace mlmoldels

BOOST_AUTO_TEST_CASE(test_extract_column_rand_data)
{
    const auto data = local::test_data();
    //  we would like to have all the data from here, since we would manipulate all the data
    const  auto data_mat = classifier::data_matrix(data, "no such column", local::fith.size(), 0u);
#if 0
    auto data_to_rand = classifier::data_matrix(data, "no such column", local::fith.size(), 0u);
    BOOST_TEST_REQUIRE(mlmodels::rows(data_mat) == mlmodels::rows(data_to_rand));
    BOOST_TEST_REQUIRE(mlmodels::columns(data_mat) == mlmodels::columns(data_to_rand));
    BOOST_TEST_REQUIRE(mlmodels::columns(data_mat) == 5u);
    BOOST_TEST_REQUIRE(mlmodels::rows(data_to_rand) == local::first.size());
    BOOST_TEST_REQUIRE((data_to_rand == data_mat), "we are expecting to have the two matrices with the same data");
#endif
    auto data_to_rand = local::test_data();
    auto after_rand = classifier::randomalized(std::move(data_to_rand));
    const auto new_data = classifier::data_matrix(after_rand, "no such column", local::fith.size(), 0u);
    BOOST_TEST_REQUIRE(mlmodels::rows(new_data) == mlmodels::rows(data_mat));
    BOOST_TEST_REQUIRE(mlmodels::columns(new_data) == mlmodels::columns(data_mat));
    BOOST_TEST_REQUIRE((new_data != data_mat), "the two mateix of data should not be the same, they don't have the same order any more");
    BOOST_TEST_REQUIRE(mlmodels::comp_rows(data_mat, new_data), "we are expecting that while we don't have the same order we have the same rows");
}
