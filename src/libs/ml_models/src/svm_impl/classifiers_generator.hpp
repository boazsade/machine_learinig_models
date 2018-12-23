#pragma once
#include "classifiers_generator.h"
#include "type_convertion.hpp"
#include <svm.h>

namespace mlmodels
{
namespace svm
{
namespace classifier
{
model_type create_model(c_rbf_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, rbf_args::value(args, columns(data)),
        0, args.cache_size, args.stop_crit,
        args.cost, 0, NULL, NULL, 0, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(nu_rbf_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, rbf_args::value(args, columns(data)),
        0, args.cache_size, args.stop_crit,
        0, 0, NULL, NULL, args.nu, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(one_rbf_train&& args, 
         const training_data& data, const class_data& targets)
{
    return create_model(static_cast<nu_rbf_train>(args),
                        data, targets);
}

model_type create_model(c_sig_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, rbf_args::value(args, columns(data)),
        args.coef0, args.cache_size, args.stop_crit,
        args.cost, 0, NULL, NULL, 0, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}


model_type create_model(nu_sig_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, rbf_args::value(args, columns(data)),
        args.coef0, args.cache_size, args.stop_crit,
        0, 0, NULL, NULL, args.nu, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(one_sig_train&& args, 
         const training_data& data, const class_data& targets)
{
    return create_model(static_cast<nu_sig_train&&>(args), data, targets);
}

model_type create_model(c_poly_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        args.degree, rbf_args::value(args, columns(data)),
        args.coef0, args.cache_size, args.stop_crit,
        args.cost, 0, NULL, NULL, 0, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(nu_poly_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        args.degree, rbf_args::value(args, columns(data)),
        args.coef0, args.cache_size, args.stop_crit,
        0, 0, NULL, NULL, args.nu, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(one_poly_train&& args, 
         const training_data& data, const class_data& targets)
{
    return create_model(static_cast<nu_poly_train&&>(args), data, targets);
}

model_type create_model(c_linear_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, 0,
        0, args.cache_size, args.stop_crit,
        args.cost, 0, NULL, NULL, 0, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(nu_linear_train&& args, 
         const training_data& data, const class_data& targets)
{
    svm_parameter params {
        type_convert(args.type), kernel_convertion(args.kernel), 
        0, 0,
        0, args.cache_size, args.stop_crit,
        0, 0, NULL, NULL, args.nu, 0,
        (int)args.shrinking, (int)args.probability
    };
    return create(params, data, targets);
}

model_type create_model(one_linear_train&& args, 
         const training_data& data, const class_data& targets)
{
    return create_model(static_cast<nu_linear_train&&>(args), data, targets);
}

}   // end of namespace classifier
}   // end of namesapce svm
}   // end of mlmodels
