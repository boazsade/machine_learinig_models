#pragma once
#include "running_mode.h"
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/sources_handling/row_data_input.h"

//namespace classifier
//{

actions::model_samples preform_action(const test_run<percent_split>& ps, 
                                    const classifier::read_result& data);

actions::model_samples preform_action(const test_run<k_fold>& ps, 
                                    const classifier::read_result& data);

//}   // end of namespace classifier

