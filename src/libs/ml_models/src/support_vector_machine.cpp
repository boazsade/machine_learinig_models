#include "libs/ml_models/support_vector_machine.h"
#include "svm_impl/svm_model.h"
#include "svm_impl/classifiers_generator.h"
#include "svm_impl/regressions_generator.h"
//#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv2/core.hpp>
#include <opencv/ml.h>
#include <iostream>
#include <boost/assign.hpp>
#include <map>
//#define _PRINT_MODEL_PARAMS
#ifdef _PRINT_MODEL_PARAMS
#include <boost/range/algorithm/copy.hpp>
#include <iterator>
#endif // _PRINT_MODEL_PARAMS
namespace mlmodels
{

namespace 
{

support_vector_machine::model_type
    generate_reg_model(const training_data& data,
                const class_data& classes, const regression_svm::args& a)
{
    if (a.type != svm::type_t::NU_SVR) {
        return support_vector_machine::model_type{};
    }
    switch (a.kernel) {
        case svm::kernel_type::LINEAR:
            return svm::regression::create_model(
                    svm::regression::nu_linear_train{}, data, classes
                );
        case svm::kernel_type::POLY:
            return svm::regression::create_model(
                    svm::regression::nu_poly_train{}, data, classes
                );
        case svm::kernel_type::RBF:
            return svm::regression::create_model(
                        svm::regression::nu_rbf_train{}, data, classes
                    );
        case svm::kernel_type::SIGMOID:
            return svm::regression::create_model(
                        svm::regression::nu_sig_train{}, data, classes
                    );
        default:
            return support_vector_machine::model_type{};
    }
}

support_vector_machine::model_type
    generate_class_model(const training_data& data,
                const class_data& classes, const classifier_svm::args& a)
{
    if (a.type != svm::type_t::C_SVC) {
        return support_vector_machine::model_type{};
    }
    switch (a.kernel) {
        case svm::kernel_type::LINEAR:
            return svm::classifier::create_model(
                    svm::classifier::c_linear_train{}, data, classes
                );
        case svm::kernel_type::POLY:
            return svm::classifier::create_model(
                    svm::classifier::c_poly_train{}, data, classes
                );
        case svm::kernel_type::RBF:
            return svm::classifier::create_model(
                        svm::classifier::c_rbf_train{}, data, classes
                    );
        case svm::kernel_type::SIGMOID:
            return svm::classifier::create_model(
                        svm::classifier::c_sig_train{}, data, classes
                    );
        default:
            return support_vector_machine::model_type{};
    }
}

}   // end of local namespce

svm::kernel_type 
    support_vector_machine::string2kernel(const std::string& from)
{
    return svm::str2kernel(from);
}

svm::type_t 
    support_vector_machine::string2type(const std::string& from)
{
    return svm::str2type(from);
}

classifier_svm::model_type 
classifier_svm::train(const training_data& data,
        const class_data& classes, const args& a) const
{
    return generate_class_model(data, classes, a);
}

support_vector_machine::_args::_args(svm::kernel_type k, svm::type_t t) :
                kernel(k), type(t)
{
}

support_vector_machine::model_type support_vector_machine::load(const char* from)
{
    return svm::load(from);
}

value_type support_vector_machine::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return svm::predict(model, samples);
    }
    return missing_value<value_type>();
}

bool support_vector_machine::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        predications = svm::predict(model, data);
        return not predications.empty();
    } else {
        return false;
    }
}

support_vector_machine::row_output support_vector_machine::test(model_type model, 
        const testing_data& data, int ) const
{
    if (model) {
        return svm::test(model, data);
    } else {
        return row_output{};
    }
}

void save_model(support_vector_machine::model_type model, const char* path)
{
    if (model) {
        svm::save(model, path);
    }
}

classifier_svm::args::args() : base_t{}
{
    type = svm::C_SVC;
}

classifier_svm::args::args(svm::kernel_type k) : base_t{k, svm::C_SVC}
{
}

classifier_svm::row_output classifier_svm::test(model_type model, const testing_data& data,
            int classes) const
{
    return base_t::test(model, data, classes);
}

regression_svm::args::args() :
        base_t{}
{
    type = svm::NU_SVR;
}

regression_svm::args::args(svm::kernel_type k) : base_t{k, svm::NU_SVR}
{
}

regression_svm::model_type regression_svm::train(const training_data& data,
            const class_data& classes, const args& a) const
{
    return generate_reg_model(data, classes, a);
}

regression_svm::row_output regression_svm::test(model_type model, const testing_data& data, int ) const
{
    return base_t::test(model, data, -1);
}

}   // end of namespace mlmodels

