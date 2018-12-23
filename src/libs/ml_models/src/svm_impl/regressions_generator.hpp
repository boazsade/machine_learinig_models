#pragma once
#include "regressions_generator.h"
#include "type_convertion.hpp"
//#include "params_io.h"
#include <svm.h>
//#include <iostream>

namespace mlmodels
{
namespace svm
{
namespace regression
{
model_type create_model(epsilon_linear_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, 0, 0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        0, args.epsilon, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(nu_linear_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, 0, 0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        args.nu, 0, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(epsilon_rbf_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, rbf_args::value(args, columns(data)), 0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        0, args.epsilon, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(nu_rbf_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        3, rbf_args::value(args, columns(data)), 0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        args.nu, 0, args.shrinking, args.probability
    };
 //   std::cout<<"create_model(nu_rbf_train& params =\n"<<param<<std::endl;
    return create(param, data, targets);
}

model_type create_model(epsilon_sig_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 0, 
        rbf_args::value(args, columns(data)), args.coef0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        0, args.epsilon, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(nu_sig_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 0, 
        rbf_args::value(args, columns(data)), args.coef0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        args.nu, 0, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(epsilon_poly_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        args.degree, rbf_args::value(args, columns(data)), args.coef0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        0, args.epsilon, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

model_type create_model(nu_poly_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter param{
        type_convert(args.type), kernel_convertion(args.kernel), 
        args.degree, rbf_args::value(args, columns(data)), args.coef0,
        args.cache_size, args.stop_crit, 
        args.cost, 0, NULL, NULL,
        args.nu, 0, args.shrinking, args.probability
    };
    return create(param, data, targets);
}

}   // end of namespace regression
}   // end of namesapce svm
}   // end of mlmodels
