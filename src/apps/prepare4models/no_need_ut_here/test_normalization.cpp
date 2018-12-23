#include <boost/test/unit_test.hpp>
#include "shared_operations.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "libs/cleanup_operatiosn/no_normalized_task.h"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "libs/cleanup_operatiosn/normalized_task.h"
#include <cmath>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE( test_no_normalization_needed )
{
    // test that you don't do anything when you don't need to do it
    auto input_data = make_test_data(24u, "test input");
    auto ret = preform_action(normalized_nothing, input_data);
    BOOST_TEST(input_data == data(ret), 
            "we do expect that we would not change anything in this case");
}

BOOST_AUTO_TEST_CASE( test_normalized_empty )
{
    normalized_data::arg_type input;
    z_normaliztion_conf_stats znorm;
    auto ret = preform_action(znorm, input);
    const auto& data_after_norm = data(ret);
    BOOST_TEST(data_after_norm == input, "empty data set, no action");
    max_normalization_stats mnorm;
    ret = preform_action(mnorm, input);
    const auto& data_after_norm2 = data(ret);
    BOOST_TEST(data_after_norm2 == input, "we expecting that we do nothing on empty set");
    max_normalization_stats mxnorm;
    ret = preform_action(mxnorm, input);
    const auto& data_after_norm3 = data(ret);
    BOOST_TEST(data_after_norm3 == input, "we expecting that we do nothing on empty set");
    minmax_normalization_stats mmnorm;
    ret = preform_action(mmnorm, input);
    const auto& data_after_norm4 = data(ret);
    BOOST_TEST(data_after_norm4 == input, "we expecting that we do nothing on empty set");
}

