#pragma once
#include <string>

namespace mlmodels
{
namespace svm
{
    enum kernel_type {
            CUSTOM, 
            LINEAR,
            POLY,
            RBF,    // default
            SIGMOID,
            CHI2,
            INTER
    };

    enum type_t {
            C_SVC,  // classifier
            NU_SVC,
            ONE_CLASS,
            EPS_SVR,
            NU_SVR  // regression
    };

    type_t str2type(const std::string& from);
    kernel_type str2kernel(const std::string& from);
    std::string to_string(kernel_type k);
    std::string to_string(type_t t);

}   // end of namespace svm
}   // end of namespace mlmodels


