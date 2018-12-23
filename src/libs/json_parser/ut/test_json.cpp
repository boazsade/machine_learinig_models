#define BOOST_TEST_MODULE TEST_MODEL_JSON
#include <boost/test/unit_test.hpp>
#include "libs/json_parser/json_istream.h"
#include "libs/json_parser/json_ostream.h"
#include <string>

BOOST_AUTO_TEST_CASE(test_basic_input) 
{
    const std::string invalid = ",,,,,---555####";
    try {
        json::istream_root p(invalid);
        BOOST_TEST((!p), "this should fail to parser as valid JSON input");
    } catch (const std::exception& ){
    } catch (...) {
        BOOST_TEST(false, "unknown exception thrown, we expecting some standard exception");
    }
}

