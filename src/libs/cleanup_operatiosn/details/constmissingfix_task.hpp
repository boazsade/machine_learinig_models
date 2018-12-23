#pragma once
#include "libs/cleanup_operatiosn/constmissingfix_task.h"
#include "logging.h"
#include <algorithm>

fix_missing::result_type preform_action(const _fix_nothing& , fix_missing::arg_type arg)
{
    LOG_DEBUG<<"not appling any fix - no need";
    return arg;
}

fix_missing::result_type preform_action(const const_value_fix& action, fix_missing::arg_type arg)
{
    if (empty(arg)) {
        LOG_INFO<<"dataset is empty, cannot apply action of fixing with "<<action;
        return arg;
    }
    auto fix_value = action.value;
    LOG_INFO<<"fix missing values with const value of "<<fix_value<<" on "<<action.invalid_rows.size()<<" rows";
    std::for_each(std::begin(action.invalid_rows), std::end(action.invalid_rows), [fix_value, &arg](auto row_num) {
                    auto i = find(arg, row_num);
                    if (i != end(arg)) {    // make sure that we have this row number
                        i->value = fix_value;
                    }
                }
    );
    return arg;
}

