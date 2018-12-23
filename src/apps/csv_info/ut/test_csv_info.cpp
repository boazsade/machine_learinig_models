#define BOOST_TEST_MODULE TEST_DATA_CLEAN
#include <boost/test/unit_test.hpp>
#include "apps/csv_info/csv_info.h"
#include "apps/csv_info/errors_types.h"
#include "apps/csv_info/test_number.h"
#include "ut_details/disable_log.h"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_CASE( test_use_case_info_msg )
{
    do_disable_log();

    std::string invalid_csv;

    std::istringstream strm{invalid_csv};
    csv_info info;
    try {
        auto ret = csv_info::process(strm, info, ',', '"', 0u);
        BOOST_TEST(ret == false, "we didn't pass any CSV, so it should fail");
        BOOST_TEST(info.empty(), "there should be nothing that we saved from the empty string");
        BOOST_TEST(info.columns_count() == 0, "there are no columns that we expecting here");
    } catch (const csv_parsing_error&) {
        BOOST_TEST(false, "we are expecting to have empty file error");
    } catch (const empty_input_file&) {
    } catch (...) {
        BOOST_TEST(false, "we are not expcting this type of error");
    }

}

BOOST_AUTO_TEST_CASE(test_number_input)
{
    //BOOST_TEST(is_number("bad") == false, "the string bad is not a number"); 
    // note that the above is an issue that we don't know how to resolve it!!
    BOOST_TEST(is_number("good") == false, "the string good is not a number");
    BOOST_TEST(is_number("1"), "1 is a number");
    BOOST_TEST(is_number("1.1"), "1.1 is a number");
}
