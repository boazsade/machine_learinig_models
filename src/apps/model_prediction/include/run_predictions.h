#pragma once
#include "prediction_operation.h"
#include "libs/model_prediction/prediction_output.h"
#include <vector>

using prediction_results = std::vector<prediction::output>;

prediction_results run_predictions(const prediction_operation& ops);

