#include <boost/test/unit_test.hpp>
#include "libs/experiments/cm_results.h"
#include "libs/experiments/confusion_matric.h"
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using input_data = std::tuple<exprs::value_type, exprs::value_type>;

inline constexpr exprs::value_type measured(const input_data& in) {
    return std::get<0>(in);
}

inline constexpr exprs::value_type predict(const input_data& in) {
    return std::get<1>(in);
}

std::ostream& operator << (std::ostream& os, const input_data& in) {
    return os<<"measure: "<<measured(in)<<", predicted: "<<predict(in);
}

using test_input_t = std::vector<input_data>;

BOOST_AUTO_TEST_CASE(test_all_true_cm)
{
    exprs::cm_results output;
    BOOST_TEST_REQUIRE(output.true_pos == 0u);
    BOOST_TEST_REQUIRE(output.true_neg == 0u);
    BOOST_TEST_REQUIRE(output.false_pos == 0u);
    BOOST_TEST_REQUIRE(output.false_neg == 0u);
    // test all true positive
    const test_input_t test_data{
        std::make_tuple(1.f, 1.f),
        std::make_tuple(1.f, 1.f),
        std::make_tuple(1.f, 1.f),
        std::make_tuple(1.f, 1.f)
    };

    output = std::accumulate(std::begin(test_data), std::end(test_data), output, [](const auto& t, const auto& v) {
                        return t + exprs::binclass::cm::calculate(measured(v), predict(v));
                    }
            );

    BOOST_TEST_REQUIRE(output.true_pos == test_data.size());
    BOOST_TEST_REQUIRE(output.true_neg == 0u);
    BOOST_TEST_REQUIRE(output.false_pos == 0u);
    BOOST_TEST_REQUIRE(output.false_neg == 0u);
}

BOOST_AUTO_TEST_CASE(test_all_true_negative)
{
    exprs::cm_results output;
    // test all true negative
    const test_input_t true_negatives{
        std::make_tuple(0.f, 0.f),
        std::make_tuple(0.f, 0.f),
        std::make_tuple(0.f, 0.f),
        std::make_tuple(0.f, 0.f)
    };
    output = std::accumulate(std::begin(true_negatives), std::end(true_negatives), output, [](const auto& t, const auto& v) {
                        return t + exprs::binclass::cm::calculate(measured(v), predict(v));
                    }
            );
    BOOST_TEST_REQUIRE(output.true_neg == true_negatives.size());
    BOOST_TEST_REQUIRE(output.true_pos == 0u);
    BOOST_TEST_REQUIRE(output.false_pos == 0u);
    BOOST_TEST_REQUIRE(output.false_neg == 0u);
}

BOOST_AUTO_TEST_CASE(test_all_false_negative)
{
    exprs::cm_results output;
    // test all true negative
    const test_input_t false_negatives{
        std::make_tuple(1.f, 0.f),
        std::make_tuple(1.f, 0.f),
        std::make_tuple(1.f, 0.f),
        std::make_tuple(1.f, 0.f)
    };
    output = std::accumulate(std::begin(false_negatives), std::end(false_negatives), output, [](const auto& t, const auto& v) {
                        return t + exprs::binclass::cm::calculate(measured(v), predict(v));
                    }
            );
    BOOST_TEST_REQUIRE(output.false_neg == false_negatives.size());
    BOOST_TEST_REQUIRE(output.true_pos == 0u);
    BOOST_TEST_REQUIRE(output.false_pos == 0u);
    BOOST_TEST_REQUIRE(output.true_neg == 0u);
}

BOOST_AUTO_TEST_CASE(test_all_false_positives)
{
    exprs::cm_results output;
    // test all true negative
    const test_input_t false_positives{
        std::make_tuple(0.f, 1.f),
        std::make_tuple(0.f, 1.f),
        std::make_tuple(0.f, 1.f),
        std::make_tuple(0.f, 1.f)
    };
    output = std::accumulate(std::begin(false_positives), std::end(false_positives), output, [](const auto& t, const auto& v) {
                        return t + exprs::binclass::cm::calculate(measured(v), predict(v));
                    }
            );
    BOOST_TEST_REQUIRE(output.false_pos == false_positives.size());
    BOOST_TEST_REQUIRE(output.true_pos == 0u);
    BOOST_TEST_REQUIRE(output.true_neg == 0u);
    BOOST_TEST_REQUIRE(output.false_neg == 0u);
}

BOOST_AUTO_TEST_CASE(test_5_tp_3_fp_2_tn_1_fn)
{
    exprs::cm_results output;
    // test all true negative
    const test_input_t mix_results{
        std::make_tuple(0.f, 1.f),  // FP
        std::make_tuple(1.f, 1.f),  // TP
        std::make_tuple(0.f, 0.f),  // TN
        std::make_tuple(1.f, 1.f),  // TP
        std::make_tuple(0.f, 1.f),  // FP
        std::make_tuple(0.f, 1.f),   // FP
        std::make_tuple(1.f, 1.f),  // TP
        std::make_tuple(1.f, 1.f),  // TP
        std::make_tuple(1.f, 0.f),  // FN
        std::make_tuple(1.f, 1.f),  // TP
        std::make_tuple(0.f, 0.f)  // TN

    };
    output = std::accumulate(std::begin(mix_results), std::end(mix_results), output, [](const auto& t, const auto& v) {
                        return t + exprs::binclass::cm::calculate(measured(v), predict(v));
                    }
            );
    BOOST_TEST_REQUIRE(output.false_pos == 3u);
    BOOST_TEST_REQUIRE(output.true_pos == 5u);
    BOOST_TEST_REQUIRE(output.true_neg == 2u);
    BOOST_TEST_REQUIRE(output.false_neg == 1u);
}


