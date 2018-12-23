#include <boost/test/unit_test.hpp>
#include "utils/dc_lookup_table.h"

BOOST_AUTO_TEST_CASE(test_lookup)
{
    lookup::table no_data;
    BOOST_TEST(empty(no_data));
    BOOST_TEST(size(no_data) == 0u);
    lookup::table t;
    t.insert(1);
    BOOST_TEST(!empty(t));
    BOOST_TEST(size(t) == 1u);
    BOOST_TEST(mapped(t, 1) == 0);
    // adding the same value should not change the content
    t.insert(1);
    t.insert(1);
    t.insert(1);
    BOOST_TEST(size(t) == 1u);
    BOOST_TEST(mapped(t, 1) == 0);

}

BOOST_AUTO_TEST_CASE(test_lookup_values)
{
    lookup::table tbl;
    tbl.insert(1000);
    tbl.insert(1);
    tbl.insert(2);
    tbl.insert(1);
    tbl.insert(2);
    tbl.insert(1.1);
    BOOST_TEST(!empty(tbl));
    BOOST_TEST(size(tbl) == 4u);    // 4 unique entries
    BOOST_TEST(mapped(tbl, 1) == 1);
    BOOST_TEST(mapped(tbl, 2) == 2);
    BOOST_TEST(mapped(tbl, 1.1) == 3);
    BOOST_TEST(mapped(tbl, 1000) == 0);
}

