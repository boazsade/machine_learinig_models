#include "actions.h"


std::pair<bool, input_data> apply_action(policy_action& ac, input_data arg)
{
    return policy_action::apply(ac, arg);
}

void finding(const policy_action& pa, row_numbers& rn, row_numbers& m) 
{
    policy_action::collect(pa, rn, m);
}

