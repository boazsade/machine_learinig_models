#include <boost/test/unit_test.hpp>
#include "libs/sources_handling/row_data_input.h"
#include "libs/system/exceptions.h"
#include <string>
#include <sstream>
#include <limits>

namespace local
{
    const std::string valid_csv_input{"\"StartTime\",\"age\",\"job\",\"marital\",\"campaign\",\"index\"\n"
        "3.46303e-309,1.11324,-5.50933e+18,4.56203e+18,6.27202e+09,0\n"
        "1.42127e-311,1.11324,2.53933e+18,4.56203e+18,6.27202e+09,4\n"
        "4.07568e-309,1.11324,8.99941e+18,5.03145e+18,7.16802e+09,5\n"
        "5.21146e-309,1.11324,-5.50933e+18,5.03145e+18,6.27202e+09,7\n"
        "3.39353e-309,1.11324,-1.72619e+18,4.56203e+18,6.27202e+09,8\n"
        "2.91835e-309,1.11324,-5.50933e+18,-2.92395e+18,6.40002e+09,9\n"
        "5.33574e-309,1.11324,2.17688e+18,-2.92395e+18,6.40002e+09,10\n"
        "1.84997e-309,1.11324,8.99941e+18,4.56203e+18,6.27202e+09,11\n"
        "1.21944e-309,1.11324,-5.16608e+18,5.03145e+18,6.78402e+09,13"
    };
    const std::string invalid_csv_input{valid_csv_input + "\n1.84997e-309,1.11324,-nan,4.56203e+18,6.27202e+09,14"};
    const std::string csv_with_missing{valid_csv_input + "\n1.84997e-309,1.11324,1.1," +
        std::to_string(std::numeric_limits<double>::max()) + ",6.27202e+09,14"};
}

BOOST_AUTO_TEST_CASE(test_reading_valid_source)
{
    std::istringstream data_source{local::valid_csv_input};
    const auto sch = read_schema(data_source);
    BOOST_TEST_REQUIRE(not sch.empty());
    auto rr = classifier::read(data_source, sch);   // should succeed
    BOOST_TEST_REQUIRE(not rr.empty(), "we should be successfully reading the input");
    BOOST_TEST_REQUIRE(rr.size() == sch.size());
    BOOST_TEST(rr[0].data.size() == 9u);
}

BOOST_AUTO_TEST_CASE(test_reading_source_with_nan)
{
    std::istringstream data_source{local::invalid_csv_input};
    const auto sch = read_schema(data_source);
    BOOST_TEST_REQUIRE(not sch.empty());
    try {
        auto rr = classifier::read(data_source, sch);   // should succeed
        BOOST_TEST(false, "we are expecting to get exception at this point");
    } catch (const malformat_input&) {
    } catch (...) {
        BOOST_TEST(false, "we are exepcting to get only malformat_input expcetion");
    }
}

BOOST_AUTO_TEST_CASE(test_reading_source_with_missing)
{
    std::istringstream data_source{local::csv_with_missing};
    const auto sch = read_schema(data_source);
    BOOST_TEST_REQUIRE(not sch.empty());
    try {
        auto rr = classifier::read(data_source, sch);   // should succeed
        BOOST_TEST(false, "we are expecting to get exception at this point");
    } catch (const malformat_input&) {
    } catch (...) {
        BOOST_TEST(false, "we are exepcting to get only malformat_input expcetion");
    }
}



