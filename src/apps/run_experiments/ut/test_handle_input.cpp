#define BOOST_TEST_MODULE TEST_MODEL_EXPR
#include "ut_details/disable_log.h"
#include "libs/sources_handling/row_data_input.h"
#include <boost/test/unit_test.hpp>
#include <boost/token_functions.hpp>
#include <string>
#include <sstream>
#include <cmath>

BOOST_AUTO_TEST_CASE(test_parsing_scheme) 
{
    do_disable_log();

    const std::string csv_input{
        "\"StartTime\",\"age\",\"job\",\"marital\",\"campaign\",\"index\"\n"
            "3.46303e-309,17.3,-5.50933e+18,4.56203e+18,6.27202e+09,0"
    };
    try {
        std::istringstream input;
        auto r = read_schema(input);
        BOOST_TEST(r.empty(), "nothing to read - should be empty");
    } catch (const std::runtime_error&) {
        BOOST_TEST(false, "this should not throw - it did not read anything");
    } catch (...) {
        BOOST_TEST(false, "we are expecting runtime error");
    }

    try {
        std::string empty{""};
        std::istringstream input{empty};
        auto ret = read_schema(input);
        BOOST_TEST(ret.empty(), "should have no entries");
    } catch (const boost::escaped_list_error&) {
    } catch (const std::runtime_error&) {
    } catch (const std::exception& e) {
        BOOST_TEST(false, "got error "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "we are not expecting runtime error");
    }
    try {
        std::istringstream input(csv_input);

        auto ret = read_schema(input);
        BOOST_TEST_REQUIRE(!ret.empty(), "cannot be an empty input here");
        BOOST_TEST_REQUIRE(ret.size() == 6, "we are expecting to have 6 entries");
        BOOST_TEST(ret[0] == "StartTime");
        BOOST_TEST(ret[1] == "age");
        BOOST_TEST(ret[2] == "job");
        BOOST_TEST(ret[3] == "marital");
        BOOST_TEST(ret[4] == "campaign");
        BOOST_TEST(ret[5] == "index");
    } catch (...) {
        BOOST_TEST(false, "there should be no error in this case");
    }
}
#ifndef DUMP_ROW_MODEL_OUTPUT
BOOST_AUTO_TEST_CASE(test_parsing_data) 
{
    const std::string csv_input{
        "\"StartTime\",\"age\",\"job\",\"marital\",\"campaign\",\"index\"\n"
            "3.463,17.3,-5.509,4.562,6.272,0"
    };
    const classifier::read_result::value_type::data_type::value_type expected_values[] = {
        3.463, 17.3, -5.509, 4.562, 6.272, 0
    };

    try {
        std::istringstream empty{};
        schema_type empty_sc;
        auto ret = classifier::read(empty, empty_sc);
        BOOST_TEST_REQUIRE(ret.empty(), "we are expecting to have an empty list of columns");
    } catch (...) {
        BOOST_TEST(false, "we are not expecting to have an exception");
    }

    try {
        std::istringstream input{csv_input};
        schema_type empty_sc;
        auto ret = classifier::read(input, empty_sc);
        BOOST_TEST_REQUIRE(ret.empty(), "we are expecting to have an empty list of columns");
    } catch (...) {
        BOOST_TEST(false, "we are not expecting to have an exception");
    }

    try {
        std::istringstream input{csv_input};
        auto schema = read_schema(input);
        BOOST_TEST_REQUIRE(!schema.empty(), "we should have a valid schema");
        BOOST_TEST_REQUIRE(schema.size() == 6, "we are expecting to have 6 entries");
        auto data = classifier::read(input, schema);
        BOOST_TEST_REQUIRE(!data.empty(), "we should have a valid input");
        BOOST_TEST_REQUIRE(data.size() == schema.size(), "both schema and data size should be the same");
        for (auto i = 0u; i < data.size(); ++i) {
            BOOST_TEST(schema[i] == data[i].ID);
            BOOST_TEST_REQUIRE(data[i].data.size() == 1u);
            auto delta_exp = std::fabs(data[i].data[0] - expected_values[i]);
            BOOST_TEST_REQUIRE((delta_exp < 0.0001f), "the data "<<
                    data[i].data[0]<<" do not equals "<<
                    expected_values[i]<<" ("<<delta_exp<<")");
        }
    } catch (...) {
        BOOST_TEST(false, "we are not expecting to have an exception");
    }
}
#endif  // DUMP_ROW_MODEL_OUTPUT
