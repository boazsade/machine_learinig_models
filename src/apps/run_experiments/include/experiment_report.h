#pragma once

#include "libs/ml_models/general_types.h"
#include <vector>
#include <string>
#include <iosfwd>

struct report_value
{
    std::string            name;
    mlmodels::value_type value = 0.f;

    report_value() = default;

    report_value(const std::string& n, 
            mlmodels::value_type v);
};

std::ostream& operator << (std::ostream& os, const report_value& rv);

struct report_list
{
    using list_type = std::vector<mlmodels::value_type>;

    std::string     name;
    list_type       items;

    report_list() = default;

    template<typename It>
    report_list(const std::string& n,
            It from, It to) :
                name{n}, items(from, to)
    {
    }
};
std::ostream& operator << (std::ostream& os, const report_list& rl);

struct experiment_report
{
    using value_items = std::vector<report_value>;
    using list_items = std::vector<report_list>;

    value_items items;
    list_items  lists;
    
    experiment_report() = default;

    explicit experiment_report(value_items i);
    explicit experiment_report(list_items l);
    experiment_report(value_items i, list_items l);
    
};
std::ostream& operator << (std::ostream& os, const experiment_report& r);

