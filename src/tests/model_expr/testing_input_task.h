#pragma once
#include "running_mode.h"
#include "actions_types.h"
#include "row_data_input.h"

namespace classifier
{

actions::testing_input preform_action(const test_run<percent_split>& ps, 
                                    const classifier::read_result& data);

actions::testing_input preform_action(const test_run<k_fold>& ps, 
                                    const classifier::read_result& data);

}   // end of namespace classifier

