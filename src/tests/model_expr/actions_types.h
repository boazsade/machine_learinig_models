#pragma once
#include "libs/classfication_algos/general_types.h"
#include <tuple>

namespace actions
{

using training_input = std::tuple<classifier::models_input, classifier::class_data>;
using testing_input = classifier::models_input;
using testing_output = classifier::testing_result;
using experiment_output = classifier::class_data;

}   // end of namespace actions

