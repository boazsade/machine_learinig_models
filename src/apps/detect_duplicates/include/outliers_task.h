#pragma once

#include "duplication_task.h"
#include <iosfwd>
#include <vector>

struct outlier_report : dupliations_report
{

    using dupliations_report::dupliations_report;

    row_numbers outliers;

    iterator outliers_begin();
    iterator outliers_end();
    const_iterator outliers_begin() const;
    const_iterator outliers_end() const;
};

bool no_outliers(const outlier_report& rep);
std::size_t outliers_count(const outlier_report& rep);
void outliered_row(outlier_report& to, row_numbers::value_type row_number);

std::ostream& operator << (std::ostream& os, const outlier_report& rep);

using finiding_summery = std::vector<outlier_report>;

finiding_summery find_outliers(const app_args& args, 
        const all_duplications_report& duplications);
