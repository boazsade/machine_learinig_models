#pragma once
#include "column_stats.h"
#include <iosfwd>
#include <vector>

struct app_args;

//using duplications = std::vector<unsigned int>; // we are saving the row numbers here

struct dupliations_report : column_stats
{
    using iterator = row_numbers::iterator;
    using const_iterator = row_numbers::const_iterator;
    
    using column_stats::column_stats;

    row_numbers dups;

    iterator dups_begin();
    iterator dups_end();
    const_iterator dups_begin() const;
    const_iterator dups_end() const;
};

bool no_duplications(const dupliations_report& dr);
std::size_t duplications_count(const dupliations_report& dr);
void duplicated_row(dupliations_report& to, row_numbers::value_type row_numer);


using all_duplications_report = std::vector<dupliations_report>;

all_duplications_report find_duplications(const app_args& args);

std::ostream& operator << (std::ostream& os, const dupliations_report& report);


