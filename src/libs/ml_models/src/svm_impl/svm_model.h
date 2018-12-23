#pragma once
#include "libs/ml_models/general_types.h"
#include "libs/ml_models/svm_impl/modelfwd.h"
#include <vector>

struct svm_parameter;
namespace mlmodels
{
namespace svm
{

using predictions_list = std::vector<std::vector<double>>;

bool save(model_type model, const char* to);
model_type load(const char* from);
model_type create(const svm_parameter& params, 
            const training_data& data, const class_data& targets);
class_data test(model_type model, const testing_data& data);
predictions_list test_detailed(model_type model, const testing_data& data);
value_type predict(model_type model, const class_data& samples);
testing_result predict(model_type model, const testing_data& data);

}   // end of namesapce svm
}   // end of mlmodels

