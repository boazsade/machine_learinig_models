#include <boost/test/unit_test.hpp>
#include "libs/sources_handling/row_data_input.h"
namespace
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

BOOST_AUTO_TEST_CASE(test_extract_column_label)
{
    const auto data = test_data();
    const auto data_mat = classifier::data_matrix(data, "first", fith.size(), 0u);
    BOOST_TEST_REQUIRE(classifier::rows(data) == mlmodels::rows(data_mat));
    BOOST_TEST_REQUIRE(classifier::columns(data) == (1u + mlmodels::columns(data_mat)));
    const auto lbl = classifier::labels_data(data, "first", first.size(), 0u);
    BOOST_TEST_REQUIRE(lbl.size() == first.size());
    BOOST_TEST_REQUIRE(lbl.size() == classifier::rows(data));
    unsigned int i = 0;
    for (; i < lbl.size(); ++i) {
        BOOST_TEST(lbl[i] == first[i]);
    }
}

