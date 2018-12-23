#include "apps/run_experiments/experiment_report.h"
#include <boost/range/algorithm/copy.hpp>
#include <iterator>
#include <iostream>


report_value::report_value(const std::string& n, 
            classifier::value_type v) : name(n), value(v)
{
}


std::ostream& operator << (std::ostream& os, const report_value& rv)
{
    return os<<"name: '"<<rv.name<<"' = "<<rv.value;
}

std::ostream& operator << (std::ostream& os, const report_list& rl)
{
    os<<"name: '"<<rl.name<<"' values:\n";
    boost::copy(rl.items, std::ostream_iterator<report_list::list_type::value_type>(os, ", "));
    return os;
}
experiment_report::experiment_report(value_items i) : 
    items(std::move(i))
{
}

experiment_report::experiment_report(list_items l) : 
    lists(std::move(l))
{
}

experiment_report::experiment_report(value_items i, list_items l) :
    items(std::move(i)), lists(std::move(l))
{
}

std::ostream& operator << (std::ostream& os, const experiment_report& r)
{
    if (r.items.empty()) {
        os<<"no items";
    } else {
        boost::copy(r.items, std::ostream_iterator<experiment_report::value_items::value_type>(os, "\n"));
    }
    if (r.lists.empty()) {
        os<<" no list of items";
    } else {
        boost::copy(r.lists, std::ostream_iterator<experiment_report::list_items::value_type>(os, "\n"));
    }
    return os;
}

