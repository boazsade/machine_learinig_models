#pragma once
#include "running_mode.h"
#include "libs/model_datatypes/model_datatypes.h"
#include "libs/sources_handling/row_data_input.h"

//namespace classifier
//{

//training_input::result_type preform_action(const precent_split& ps, const training_input::input_data& data);
//training_input::result_type preform_action(const k_fold& kf, const training_input::input_data& data);
//
// split the data for testing and training
actions::training_input preform_action(const train_run<percent_split>& ps, 
                                    const classifier::read_result& data);

actions::training_input preform_action(const train_run<k_fold>& ps, 
                                    const classifier::read_result& data);

//}   // end of namespace classifier

