#pragma once
#include "classifier_params.hpp"
#include "svm_model.h"
#include "libs/ml_models/general_types.h"

namespace mlmodels
{
namespace svm
{
namespace classifier
{

model_type create_model(c_rbf_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_rbf_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(one_rbf_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(c_sig_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_sig_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(one_sig_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(c_poly_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_poly_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(one_poly_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(c_linear_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(nu_linear_train&& args, 
         const training_data& data, const class_data& targets);

model_type create_model(one_linear_train&& args, 
         const training_data& data, const class_data& targets);

}   // end of namespace classifier
}   // end of namesapce svm
}   // end of mlmodels

