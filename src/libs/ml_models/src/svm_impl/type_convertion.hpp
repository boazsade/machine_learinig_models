#pragma once
#include "libs/ml_models/svm_types.h"
#include <svm.h>

namespace mlmodels
{

constexpr int type_convert(svm::type_t t)
{
    switch (t) {
        case svm::C_SVC:
            return ::C_SVC;
        case svm::NU_SVC:
            return ::NU_SVC;
        case svm::ONE_CLASS:
            return ::ONE_CLASS;
        case svm::EPS_SVR:
            return ::EPSILON_SVR;
        case svm::NU_SVR:
            return ::NU_SVR;
    }

}

constexpr int kernel_convertion(svm::kernel_type k) {
    switch (k) {
        case svm::LINEAR:
            return ::LINEAR;
        case svm::POLY:
            return ::POLY;
        case svm::RBF:
            return ::RBF;
        case svm::SIGMOID:
            return ::SIGMOID;
        default:
            return -1;  // error
    }
}

}   // end of namespace mlmodels

