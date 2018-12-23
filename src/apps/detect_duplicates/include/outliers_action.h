#pragma once
#include "libs/sources_handling/input_data.h"
#include "column_stats.h"
#include "outliers_task.h"
#include "actions.h"
#include <utility>
#include <vector>

struct outlier_predicate
{
    using result_type = std::pair<bool, input_data>;
    using iterator = dupliations_report::const_iterator;

    outlier_predicate(iterator s, iterator l);
    outlier_predicate() = default;

    result_type apply_action(input_data d) const;

private:
    using ignore_list = row_numbers;

    ignore_list to_ignore;
};

outlier_predicate::result_type apply_action(outlier_predicate& da, input_data arg);

struct outliers_detector
{
    explicit outliers_detector(policy_action op); 

    outlier_predicate::result_type apply_action(input_data data);

    void finding(row_numbers& outliers, row_numbers& missing) const;

private:
    using data_list = std::vector<input_data>;

    data_list legal_input;
    policy_action action;
    row_numbers missings;
};

outlier_predicate::result_type apply_action(outliers_detector& da, input_data arg);
void finding(const outliers_detector& od, row_numbers& outliers, row_numbers& missing) ;

