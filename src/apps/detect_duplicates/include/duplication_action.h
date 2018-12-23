#pragma once
#include "column_stats.h"
#include "actions.h"
#include "libs/sources_handling/input_data.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"
#include <utility>
#include <set>
#include <iosfwd>

class duplication_base
{
public:
    using result_type = std::pair<bool, input_data>;

    duplication_base() = default;
    explicit duplication_base(int pre);

    double get(double from) const;

protected:
    struct comparator 
    {
        bool operator () (const input_data& left, const input_data& right) const;
    };

    using values_list = std::set<input_data, comparator>;

    values_list lookup;
private:
    int precistion = 0;
};

class duplication_first : public duplication_base
{
public:
    using duplication_base::duplication_base;

    duplication_base::result_type apply_action(input_data arg) ;
};

std::pair<bool, input_data> apply_action(duplication_first& da, input_data arg);

class duplication_last : public duplication_base
{
public:
    using duplication_base::duplication_base;

    duplication_base::result_type apply_action(input_data arg) ;
};

std::pair<bool, input_data> apply_action(duplication_last& da, input_data arg);

class duplication_action
{
public:
    using rows_list = std::set<std::size_t>;
    using rows_list_p = std::shared_ptr<rows_list>;

    explicit duplication_action(policy_action a);

    duplication_base::result_type apply_action(input_data arg) ;

    void finding(row_numbers& dup_list, row_numbers& missing) const;
private:

    policy_action    policy;
    row_numbers      duplications;
    row_numbers      missings;
};

std::pair<bool, input_data> apply_action(duplication_action& da, input_data arg);
void finding(const duplication_action& from, row_numbers& to, row_numbers& missing);


