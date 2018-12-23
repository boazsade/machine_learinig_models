#define BOOST_TEST_MODULE TEST_MODEL_JSON
#include <boost/test/unit_test.hpp>
#include "libs/csv_parser/csv_parser.h"
#include <sstream>

BOOST_AUTO_TEST_CASE(test_basic_input) 
{
    std::istringstream invalid;
    csv::parser::source_type src{invalid};
    try {
        csv::parser p; 
        BOOST_TEST(p.good(), "initial state is good");
        auto i = p.process(src);
        BOOST_TEST(i == 0, "this should not produce anything");
        BOOST_TEST(p.eof(), "we don't have any more data to process");
        std::string t = p.get_token() ;
        BOOST_TEST(t.empty(), "we should not have data in the token");
    } catch (const std::exception& ){
    } catch (...) {
        BOOST_TEST(false, "unknown exception thrown, we expecting some standard exception");
    }
}

