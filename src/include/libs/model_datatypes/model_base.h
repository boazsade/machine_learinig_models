#pragma once
#include "model_datatypes.h"

struct model_base
{
    using training_data = actions::training_data;
    using testing_input = actions::testing_input;
    using test_output = actions::testing_output;
    using labels_type =  mlmodels::class_data;
    using row_type = mlmodels::class_data;
    using prediction_value = mlmodels::value_type;
};

