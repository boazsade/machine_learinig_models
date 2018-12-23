#ifndef DUMP_ROW_MODEL_OUTPUT
#include <boost/test/unit_test.hpp>
#include "libs/ml_models/utilities.h"
#include <algorithm>

BOOST_AUTO_TEST_CASE(convert_classes)
{
    // note - all are unique values, we want to see if we are getting the same number of elements
    mlmodels::class_data input{
        1.1f, 2.111f, 33.1f, 444.5f, 23.5555f
    };

    auto classes = mlmodels::classes_transform(std::begin(input), std::end(input));
    BOOST_TEST_REQUIRE(classes.size() == input.size());
    BOOST_TEST_REQUIRE(classes[0] == 1);
    BOOST_TEST_REQUIRE(classes[classes.size()-1] == (int)input.size());
}

BOOST_AUTO_TEST_CASE(convert_classes_single)
{
    mlmodels::class_data input(10, 555.666f);  // 10 elements all the same value

    auto classes = mlmodels::classes_transform(std::begin(input), std::end(input));
    BOOST_TEST_REQUIRE(classes.size() == input.size());
    BOOST_TEST_REQUIRE(classes.size() == 10u);
    BOOST_TEST_REQUIRE(classes[0] != input[0]);
    BOOST_TEST_REQUIRE(classes[0] == 1);
    BOOST_TEST_REQUIRE(classes[classes.size()-1] == classes[0]);
    BOOST_TEST_REQUIRE((std::all_of(std::begin(classes), std::end(classes), [](auto val) { return val == 1; } )), "all values must be the same value 1");
}

BOOST_AUTO_TEST_CASE(order_classes_test)
{
    mlmodels::class_data input(10, 555.666f);  // 10 elements all the same value
    input[3] = 1.43;    // so that we would have a single entry that is different in a given location
    auto classes = mlmodels::classes_transform(std::begin(input), std::end(input));

    BOOST_TEST_REQUIRE(classes.size() == input.size());
    BOOST_TEST_REQUIRE(classes.size() == 10u);
    BOOST_TEST_REQUIRE(classes[0] != input[0]);
    BOOST_TEST_REQUIRE(classes[0] == 1);
    BOOST_TEST_REQUIRE(classes[classes.size()-1] == classes[0]);
    BOOST_TEST_REQUIRE(classes[3] != classes[0]); 
    BOOST_TEST_REQUIRE(classes[3] == 2);
}

BOOST_AUTO_TEST_CASE(order_classes_multi_test)
{
    mlmodels::class_data input(10, 555.666f);  // 10 elements all the same value
    input[3] = 1.43;    // so that we would have a single entry that is different in a given location
    input[5] = 1.43;
    input[7] = 1.434;   // make another unique value here
    auto classes = mlmodels::classes_transform(std::begin(input), std::end(input));

    BOOST_TEST_REQUIRE(classes.size() == input.size());
    BOOST_TEST_REQUIRE(classes.size() == 10u);
    BOOST_TEST_REQUIRE(classes[0] != input[0]);
    BOOST_TEST_REQUIRE(classes[0] == 1);
    BOOST_TEST_REQUIRE(classes[classes.size()-1] == classes[0]);
    BOOST_TEST_REQUIRE(classes[3] != classes[0]); 
    BOOST_TEST_REQUIRE(classes[3] == 2);
    BOOST_TEST_REQUIRE(classes[3] == classes[5]);
    BOOST_TEST_REQUIRE(classes[3] != classes[7]);
    BOOST_TEST_REQUIRE(classes[0] != classes[7]);
    BOOST_TEST_REQUIRE(3 == classes[7]);
}

BOOST_AUTO_TEST_CASE(test_make_transformer)
{
    mlmodels::class_data input{1.1f, 2.111f, 33.1f, 444.5f, 23.5555f}; // all different
    auto transofer = mlmodels::make_transformer(std::begin(input), std::end(input));
    BOOST_TEST_REQUIRE(transofer.size() == input.size());
    mlmodels::class_data input_same(40u, 234.23f);
    transofer = mlmodels::make_transformer(std::begin(input_same), std::end(input_same));
    BOOST_TEST_REQUIRE(transofer.size() == 1u);
    BOOST_TEST_REQUIRE(transofer.begin()->second == 1); // only one element and it must be one
}

#endif  // DUMP_ROW_MODEL_OUTPUT
