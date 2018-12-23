#define BOOST_TEST_MODULE TEST_DATA_CLEAN
#include <boost/test/unit_test.hpp>
#include "shared_operations.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include "libs/sources_handling/source_data.h"
#include "ut_details/disable_log.h"
#include <boost/iterator/counting_iterator.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>


BOOST_AUTO_TEST_CASE( test_cleanup_no_action )
{
    do_disable_log();
    source_data::id_type id = "some id";

    source_data test_data = make_test_data(20, id);

    BOOST_TEST(preform_action(dont_drop, test_data) == test_data);
}

BOOST_AUTO_TEST_CASE( test_cleanup_no_drops )
{
    source_data::id_type id = "some id";

    source_data test_data = make_test_data(20, id);
    filterout_conf conf;

    BOOST_TEST(preform_action(conf, test_data) == test_data);
}

BOOST_AUTO_TEST_CASE( test_cleanup_drop_one )
{
    source_data::id_type id = "some id";
    auto test_input =  make_test_data(2, id);
    filterout_conf conf{id, boost::make_counting_iterator(1),
                        boost::make_counting_iterator(2)
    };
    auto result = preform_action(conf, test_input);
    auto row = test_input.row_data()[1];
    BOOST_TEST(result != test_input);
    BOOST_TEST(size(result) == 1);
    BOOST_TEST(result.row_data()[0].row != 1);
    BOOST_TEST(result.row_data()[0] == row);

    filterout_conf conf2{id, boost::make_counting_iterator(0),
        boost::make_counting_iterator(1)
    };
    result = preform_action(conf2, test_input);
    BOOST_TEST(result == test_input);
    BOOST_TEST(result.row_data()[0].row == 1);
    BOOST_TEST(size(result) == 2);
    BOOST_TEST(result.row_data()[0] != row);
    BOOST_TEST(result.row_data()[1] == row);
}

BOOST_AUTO_TEST_CASE( test_cleanup_drop_all )
{
    source_data::id_type id = "some id";
    auto test_input =  make_test_data(6, id);
    filterout_conf conf{id, boost::make_counting_iterator(1),
                        boost::make_counting_iterator(7)
    };
    auto result = preform_action(conf, test_input);
    BOOST_TEST(result != test_input);
    BOOST_TEST(size(result) == 0);
}

BOOST_AUTO_TEST_CASE( test_cleanup_drop_last )
{
    source_data::id_type id = "some id";
    auto test_input =  make_test_data(6, id);
    filterout_conf conf{id, boost::make_counting_iterator(6),
                        boost::make_counting_iterator(7)
    };
    auto result = preform_action(conf, test_input);
    BOOST_TEST(result != test_input);
    BOOST_TEST(size(result)+1 == size(test_input));
    BOOST_TEST(result.row_data()[0] == test_input.row_data()[0]);
    BOOST_TEST(result.row_data()[1] == test_input.row_data()[1]);
    BOOST_TEST(result.row_data()[2] == test_input.row_data()[2]);
    BOOST_TEST(result.row_data()[3] == test_input.row_data()[3]);
    BOOST_TEST(result.row_data()[4] == test_input.row_data()[4]);
}
#if 0
std::ostream& operator << (std::ostream& os, const input_data& sd)
{
    return os<<"{"<<sd.row<<":"<<sd.value<<"}";
}
#endif
