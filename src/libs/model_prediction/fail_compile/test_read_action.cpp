#define BOOST_TEST_MODULE TEST_MODEL_USE_MODEL
#include <boost/test/unit_test.hpp>
#include "libs/model_prediction/prediction_flow.h"
#include "libs/model_prediction/prediction_data.h"
#include "libs/model_prediction/prediction_row.h"
#include "libs/model_prediction/prediction_output.h"
#include "libs/model_prediction/prediction_status.h"
#include "libs/system/exceptions.h"
#include "libs/sources_handling/map_schema.h"
#include "libs/sources_handling/schema_read.hpp"
#include "utils/algorithms.hpp"
#include "ut_details/disable_log.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

namespace 
{

const char* input_file = "./test_data.csv";

schema_type load_schema(std::istream& from)
{
    return sr::read_schema(from, " \t", [](auto token) { return token; });
}

schema_mapper get_schema(std::istream& input)
{
    auto sd{load_schema(input)};
    return schema_mapper{std::begin(sd), std::end(sd)};
}

schema_mapper get_schema()
{
    using namespace std::string_literals;
    std::ifstream input{input_file};
    if (!input) {
        throw std::runtime_error{"failed to open "s + input_file + " for reading"s};
    }
    return get_schema(input);
}


}   // end of local namespace

BOOST_AUTO_TEST_CASE(test_no_data) 
{
    do_disable_log();
    try {
        prediction::data empty_data{prediction::output{prediction::row_number{22u}, prediction::status::OK}};
        BOOST_TEST(empty(empty_data));
        BOOST_TEST(size(empty_data) == 0u);
        BOOST_TEST(bad(empty_data));
        BOOST_TEST(not good(empty_data));
    } catch (const std::exception& e){
        BOOST_TEST(false, "an error while procesing empty case "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "unknown exception thrown, we expecting some standard exception");
    }
}

BOOST_AUTO_TEST_CASE(test_invalid_input_to_read) 
{
    try {
        const auto scheme = get_schema();
        BOOST_TEST(not scheme.empty());
        std::ifstream invalid_stream{"not-such-file"};
        BOOST_TEST(not invalid_stream.good());
        auto not_read_stream_data{prediction::read(prediction::read_row{scheme}, invalid_stream)};
        BOOST_TEST(empty(not_read_stream_data));
        BOOST_TEST(size(not_read_stream_data) == 0u);
        BOOST_TEST(bad(not_read_stream_data));
        BOOST_TEST(not good(not_read_stream_data));
        BOOST_TEST(state(prediction::current_state(not_read_stream_data)) == prediction::status::END_OF_FILE);
    } catch (const std::exception& e) {
        BOOST_TEST(false, "an error while trying to read from an empty stream "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "error while trying to read from empty stream - uknown");
    }
}

BOOST_AUTO_TEST_CASE(read_one_line_and_verify_input)
{
    try {
        const std::string titles{"1 2   3   4   5   index"};
        const prediction::row::data_type row_data[] = {23.7f, 44.6f, 90.1f, 33.2f, 22.3};
        std::string rowstr;
        rowstr = std::accumulate(std::begin(row_data), std::end(row_data), rowstr, [](auto str, auto num) {
                        return str + std::to_string(num) + "\t";
                }
        );
        const auto data_input = titles + "\n" + rowstr + std::to_string(1u);
        //std::cout<<"the data for the test is {"<<data_input<<"}\n";
        std::istringstream input_stream{data_input};
        const auto scheme = get_schema(input_stream);
        BOOST_TEST_REQUIRE(not scheme.empty());
        BOOST_TEST_REQUIRE(scheme.columns_num() == utils::size(row_data) + 1u);
        // read the row from the stream - we have one valid row that we are reading here..
        auto row{prediction::read(prediction::read_row{scheme}, input_stream)};
        BOOST_TEST(not empty(row));
        BOOST_TEST(size(row) + 1u == scheme.columns_num());
        BOOST_TEST(good(row));
        BOOST_TEST(not bad(row));
        BOOST_TEST(state(prediction::current_state(row)) == prediction::status::OK);
        const auto& r = prediction::current_values(row);
        BOOST_TEST_REQUIRE(r.size() == utils::size(row_data));
        for (unsigned int i = 0; i < r.size(); ++i) {
            BOOST_TEST(round_cmp(r[i], row_data[i], 0.001f));
        }
        // we would try to read the next line, but since this is empty now, it should fail
        auto no_more_data{prediction::read(prediction::read_row{scheme}, input_stream)};
        BOOST_TEST_REQUIRE(empty(no_more_data));
        BOOST_TEST(state(prediction::current_state(no_more_data)) == prediction::status::END_OF_FILE);
    } catch (const std::exception& e) {
        BOOST_TEST(false, "an error while trying to read from an empty stream "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "error while trying to read from empty stream - uknown");
    }
}

BOOST_AUTO_TEST_CASE(read_few_rows)
{
    try {
        std::ifstream input_stream{input_file};
        BOOST_TEST_REQUIRE(input_stream.good());
        const auto scheme = get_schema(input_stream);
        BOOST_TEST_REQUIRE(not scheme.empty());
        int rows2read = 7;
        prediction::read_row read_op{scheme};
        while (rows2read > 0) {
            auto row{prediction::read(read_op, input_stream)};
            BOOST_TEST(not empty(row));
            BOOST_TEST(size(row) + 1u == scheme.columns_num());
            BOOST_TEST(good(row));
            BOOST_TEST(not bad(row));
            BOOST_TEST(state(prediction::current_state(row)) == prediction::status::OK);
            --rows2read;
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "an error while trying to read from an empty stream "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "error while trying to read from empty stream - uknown");
    }
}

