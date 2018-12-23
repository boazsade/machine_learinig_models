#define BOOST_TEST_MODULE TEST_MODEL_EXPR
#include <boost/test/unit_test.hpp>
#include "libs/experiments/ROC.h"
#include <cmath>

float trancate(float input, int by)
{
    input *= by;
    return std::roundf(input)/by;
}

BOOST_AUTO_TEST_CASE(test_basic_algorithm) 
{
    constexpr exprs::value_type acc = 0.461538f;
    constexpr exprs::value_type rmse = 1.03775f;
    constexpr exprs::value_type roc_area = 0.380952f;
    const  exprs::data_array tpr{
        0.f, 0.142857f, 0.714286f, 1.f
    };

    const exprs::data_array fpr{
        0.f, 0.333333f, 0.833333f, 1.f
    };

    exprs::data_array actual_measures = {
        1.f, 0.f, 2.f, 1.f, 0.f, 1.f, 2.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f
    };

    exprs::data_array calcualted_values = {
        1.f, 1.f, 1.f, 0.f, 2.f, 1.f, 2.f, 1.f, 2.f, 0.f, 1.f, 0.f, 1.f
    };

    exprs::ROC roc_calc;
    BOOST_TEST(roc_calc.results.area == 0.f);
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
    BOOST_TEST_REQUIRE((roc_calc.calculate(actual_measures, calcualted_values)));
    BOOST_TEST(trancate(roc_calc.results.area, 4) == trancate(roc_area, 4));
    BOOST_TEST(trancate(roc_calc.results.rmse, 4) == trancate(rmse, 4));
    BOOST_TEST(trancate(roc_calc.results.accuracy, 4) == trancate(acc, 4));
    BOOST_TEST_REQUIRE(roc_calc.results.sensitivity.size() == tpr.size());
    BOOST_TEST_REQUIRE(roc_calc.results.fall_out.size() == fpr.size());
    BOOST_TEST_REQUIRE(roc_calc.results.fall_out.size() == roc_calc.results.sensitivity.size());
    for (auto i = 0u; i < roc_calc.results.sensitivity.size(); i++) {
        BOOST_TEST(trancate(roc_calc.results.sensitivity[i], 4) == trancate(tpr[i], 4));
        BOOST_TEST(trancate(roc_calc.results.fall_out[i], 4) == trancate(fpr[i], 4));
    }
}

BOOST_AUTO_TEST_CASE(test_invalid_entries)
{
    exprs::ROC roc_calc;
    BOOST_TEST_REQUIRE((roc_calc.classes_calculate(std::vector<int>{}, std::vector<float>{}) == false));
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
    BOOST_TEST_REQUIRE((roc_calc.calculate(std::vector<float>{}, std::vector<float>{}) == false));
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
    BOOST_TEST_REQUIRE((roc_calc.calculate(std::vector<float>{1.f, 2.f, 3.f}, std::vector<float>{1.f, 2.f}) == false));
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
    BOOST_TEST_REQUIRE((roc_calc.classes_calculate(std::vector<int>{1, 2, 3}, std::vector<float>{1.f, 2.f}) == false));
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
}

BOOST_AUTO_TEST_CASE(test_classifier_calc)
{
    constexpr exprs::value_type acc = 0.461538f;
    constexpr exprs::value_type rmse = 1.03775f;
    constexpr exprs::value_type roc_area = 0.380952f;
    const  exprs::data_array tpr{
        0.f, 0.142857f, 0.714286f, 1.f
    };

    const exprs::data_array fpr{
        0.f, 0.333333f, 0.833333f, 1.f
    };

    std::vector<int> actual_measures = {
        1, 0, 2, 1, 0, 1, 2, 0, 0, 1, 1, 0, 0
    };

    exprs::data_array calcualted_values = {
        1.f, 1.f, 1.f, 0.f, 2.f, 1.f, 2.f, 1.f, 2.f, 0.f, 1.f, 0.f, 1.f
    };
    exprs::ROC roc_calc;
    BOOST_TEST(roc_calc.results.area == 0.f);
    BOOST_TEST(roc_calc.results.accuracy == 0.f);
    BOOST_TEST(roc_calc.results.rmse == 0.f);
    BOOST_TEST(roc_calc.results.fall_out.empty());
    BOOST_TEST(roc_calc.results.sensitivity.empty());
    BOOST_TEST_REQUIRE((roc_calc.classes_calculate(actual_measures, calcualted_values)));
    BOOST_TEST(trancate(roc_calc.results.area, 4) == trancate(roc_area, 4));
    BOOST_TEST(trancate(roc_calc.results.rmse, 4) == trancate(rmse, 4));
    BOOST_TEST(trancate(roc_calc.results.accuracy, 4) == trancate(acc, 4));
    BOOST_TEST_REQUIRE(roc_calc.results.sensitivity.size() == tpr.size());
    BOOST_TEST_REQUIRE(roc_calc.results.fall_out.size() == fpr.size());
    BOOST_TEST_REQUIRE(roc_calc.results.fall_out.size() == roc_calc.results.sensitivity.size());
    for (auto i = 0u; i < roc_calc.results.sensitivity.size(); i++) {
        BOOST_TEST(trancate(roc_calc.results.sensitivity[i], 4) == trancate(tpr[i], 4));
        BOOST_TEST(trancate(roc_calc.results.fall_out[i], 4) == trancate(fpr[i], 4));
    }
}
