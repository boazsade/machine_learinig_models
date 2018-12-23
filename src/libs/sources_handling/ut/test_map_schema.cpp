#include <boost/test/unit_test.hpp>
#include "libs/sources_handling/map_schema.h"
#include "libs/sources_handling/schema_type.h"
#include <algorithm>
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE(test_empty_schema_case)
{
    using namespace std::string_literals;

    schema_mapper empty_mapper; 
    BOOST_TEST(empty_mapper.empty(), "we are expecting that we would not have anyting - using default constructor");
    BOOST_TEST(empty_mapper.columns_num() == 0u, "there are nothing so no columns count");
    BOOST_TEST(empty_mapper["column id"s] == schema_mapper::invalid);
    auto id_it_start = make_schemaid_iterator(empty_mapper);
    auto id_it_end = make_schemaid_end_iterator();
    BOOST_TEST((id_it_start == id_it_end), "empty range - we should have the iterators pointing to same location");
    int count = 0;
    std::for_each(id_it_start, id_it_end, [&count](const auto&) {
                ++count;
                }
    );
    BOOST_TEST(count == 0, "we should not have done any iteration");
}

BOOST_AUTO_TEST_CASE(test_single_mapped_column)
{
    using namespace std::string_literals;
    std::vector<std::string> columns{"only one column"s};
    schema_mapper one_col{std::begin(columns), std::end(columns)};
    BOOST_TEST_REQUIRE(one_col.empty() == false, "we should not have empty mapper");
    BOOST_TEST_REQUIRE(one_col.columns_num() == columns.size(), "we should have the same number of columns here as what we initiate the object with");
    BOOST_TEST_REQUIRE(one_col[columns[0]] == 0);
    auto id_it_start = make_schemaid_iterator(one_col);
    auto id_it_end = make_schemaid_end_iterator();
    BOOST_TEST((id_it_start != id_it_end), "not an empty range - we should have the iterators pointing to different location");
    int count = 0;
    std::for_each(id_it_start, id_it_end, [&count](const auto&) {
                    ++count;
                }
    );
    BOOST_TEST(count == 1, "we should do one iteration");
    id_it_start = make_schemaid_iterator(one_col);
    BOOST_TEST(*id_it_start == columns[0]);
    auto index_start = make_index_iterator(one_col);
    auto index_end = make_end_index_iterator();
    count = 0;
    BOOST_TEST(*index_start == 0, "we are expecting to have first index 0");
    std::for_each(index_start, index_end, [&count](const auto&) {
                    ++count;
                }
    );
    BOOST_TEST(count == 1, "we should do one iteration");
}

BOOST_AUTO_TEST_CASE(test_with_some_columns)
{
    using namespace std::string_literals;
    std::vector<std::string> columns{"first_column"s,
                                     "second column"s,
                                     "third column"s,
                                     "last-column"s
    };
    schema_mapper some_columns{std::begin(columns), std::end(columns)};
    BOOST_TEST_REQUIRE(some_columns.empty() == false, "we should not have empty mapper");
    BOOST_TEST_REQUIRE(some_columns.columns_num() == columns.size(), "we should have the same number of columns here as what we initiate the object with");
    auto id_it_start = make_schemaid_iterator(some_columns);
    auto id_it_end = make_schemaid_end_iterator();
    int count = 0;
    std::sort(std::begin(columns), std::end(columns));  // otherwise it would not work for the sorting
    while (id_it_start != id_it_end) {
        BOOST_TEST_REQUIRE(*id_it_start == columns[count], *id_it_start << " != "<<columns[count]<<" @"<<count);
        ++count;
        ++id_it_start;
    }
    BOOST_TEST((int)count == columns.size());
    auto index_start = make_index_iterator(some_columns);
    auto index_end = make_end_index_iterator();
    count = 0;
    std::vector<schema_mapper::column_id> expected_ids(columns.size(), 0);
    int index = 0;
    std::generate(std::begin(expected_ids), std::end(expected_ids), [&index]() { return index++; });
    while (index_start != index_end) {
        const auto val = *index_start;
        auto found = std::any_of(std::begin(expected_ids), std::end(expected_ids), [val](auto v) {
                                    return val == v;
                                }
        );
        BOOST_TEST(found, "we expecting to have "<<val<<" as one of the indices");
        ++count;
        ++index_start;
    }
    BOOST_TEST((int)count == columns.size());
}




