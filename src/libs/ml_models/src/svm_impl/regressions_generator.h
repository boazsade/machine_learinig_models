#pragma once
#include "regression_params.hpp"
#include "svm_model.h"
#include "libs/ml_models/general_types.h"

namespace mlmodels
{
namespace svm
{
namespace regression
{

model_type create_model(epsilon_linear_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_linear_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(epsilon_rbf_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_rbf_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(epsilon_sig_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_sig_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(epsilon_poly_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_poly_train&& args, 
         const training_data& data, const class_data& targets);

}   // end of namespace regression
}   // end of namesapce svm
}   // end of mlmodels

