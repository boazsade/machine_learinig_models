#define BOOST_TEST_MODULE TEST_SCORING
#include <boost/test/unit_test.hpp>
#include "libs/scores_evaluation/score_evaluation.h"

BOOST_AUTO_TEST_CASE(test_empty_data_set)
{
    actions::experiment_input empty_data_set;
    const auto output = calculate(confusion_matric_test{}, empty_data_set);
    BOOST_TEST(output.true_pos == 0u);
    BOOST_TEST(output.true_neg == 0u);
    BOOST_TEST(output.false_neg == 0u);
    BOOST_TEST(output.false_pos == 0u);
}

BOOST_AUTO_TEST_CASE(test_single_entry_true_pos)
{
    actions::experiment_input data_set{
        mlmodels::class_data{1.f},
        mlmodels::class_data{1.f}
    };
    const auto output = calculate(confusion_matric_test{}, data_set);
    BOOST_TEST(output.true_pos == 1u);
    BOOST_TEST(output.true_neg == 0u);
    BOOST_TEST(output.false_neg == 0u);
    BOOST_TEST(output.false_pos == 0u);
}

BOOST_AUTO_TEST_CASE(test_single_entry_true_neg)
{
    actions::experiment_input data_set{
        mlmodels::class_data{0.f},
        mlmodels::class_data{0.f}
    };
    const auto output = calculate(confusion_matric_test{}, data_set);
    BOOST_TEST(output.true_pos == 0u);
    BOOST_TEST(output.true_neg == 1u);
    BOOST_TEST(output.false_neg == 0u);
    BOOST_TEST(output.false_pos == 0u);
}

BOOST_AUTO_TEST_CASE(test_single_entry_false_pos)
{
    actions::experiment_input data_set{
        mlmodels::class_data{1.f},
        mlmodels::class_data{0.f}
    };
    const auto output = calculate(confusion_matric_test{}, data_set);
    BOOST_TEST(output.true_pos == 0u);
    BOOST_TEST(output.true_neg == 0u);
    BOOST_TEST(output.false_neg == 0u);
    BOOST_TEST(output.false_pos == 1u);
}

BOOST_AUTO_TEST_CASE(test_single_entry_false_neg)
{
    actions::experiment_input data_set{
        mlmodels::class_data{0.f},
        mlmodels::class_data{1.f}
    };
    const auto output = calculate(confusion_matric_test{}, data_set);
    BOOST_TEST(output.true_pos == 0u);
    BOOST_TEST(output.true_neg == 0u);
    BOOST_TEST(output.false_neg == 1u);
    BOOST_TEST(output.false_pos == 0u);
}

BOOST_AUTO_TEST_CASE(test_single_entry_4tp_2tn_1fp)
{
    actions::experiment_input data_set{
        mlmodels::class_data{1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f},
        mlmodels::class_data{1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f}
    };
    const auto output = calculate(confusion_matric_test{}, data_set);
    BOOST_TEST(output.true_pos == 4u);
    BOOST_TEST(output.true_neg == 2u);
    BOOST_TEST(output.false_neg == 0u);
    BOOST_TEST(output.false_pos == 1u);
}


