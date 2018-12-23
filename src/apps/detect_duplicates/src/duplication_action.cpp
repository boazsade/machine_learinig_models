#include "apps/detect_duplicates/duplication_action.h"
#include "utils/math_utils.h"
#include "logging.h"
#include <iostream>
#include <cmath>
#include <limits>

namespace
{

const double missing_value = std::numeric_limits<double>::max();

}   // end of local namespace


bool duplication_base::comparator::operator () (const input_data& left, const input_data& right) const
{
    return left.value < right.value;
}

std::pair<bool, input_data> apply_action(duplication_first& da, input_data arg)
{
    return da.apply_action(arg);
}

std::pair<bool, input_data> apply_action(duplication_last& da, input_data arg)
{
    return da.apply_action(arg);
}

std::pair<bool, input_data> apply_action(duplication_action& da, input_data arg)
{
    return da.apply_action(arg);
}

duplication_base::duplication_base(int res) : precistion(res)
{
}

double duplication_base::get(double val) const
{
    return utils::round_by(val, precistion);
}

duplication_base::result_type duplication_last::apply_action(input_data arg)
{
    arg.value = get(arg.value);
    auto i = lookup.find(arg);
    bool flag = true;
    if (i != lookup.end()) {
        LOG_DEBUG<<"found duplication "<<arg.value<<" at row "<<arg.row;
        auto r = i->row;
        lookup.erase(i);
        lookup.insert(arg);
        arg.row = r;
        flag = false;
    } else {
       lookup.insert(arg);
    }
    return std::make_pair(flag, arg);
}

duplication_base::result_type duplication_first::apply_action(input_data arg)
{
    arg.value = get(arg.value);
    auto i = lookup.insert(arg);
    if (!(i.second)) {    // we have it
        LOG_DEBUG<<"found duplication "<<arg.value<<" at row "<<arg.row;
        return std::make_pair(false, arg);
    } else {
        return std::make_pair(true, arg);
    }
}

duplication_action::duplication_action(policy_action a) :
    policy(a)
{
}

duplication_base::result_type duplication_action::apply_action(input_data arg)
{
    // ignore missing values
    if (arg.value == missing_value) {
        LOG_DEBUG<<"missing value, ignoring";
        missings.push_back(arg.row);
        return std::make_pair(true, arg);
    }

    auto ret = ::apply_action(policy, arg);

    if (!ret.first) {   // if we failed save it
        duplications.push_back(ret.second.row);
    }
    return ret;
}

void duplication_action::finding(row_numbers& to, row_numbers& missing) const
{
    to.insert(to.end(), std::begin(duplications), std::end(duplications));
    missing.insert(missing.end(), std::begin(missings), std::end(missings));
}

void finding(const duplication_action& from, row_numbers& to, row_numbers& missing)
{
    from.finding(to, missing);
}



