#include "params_io.h"
#include <svm.h>
#include <iostream>

const char* type2string(int type)
{
    switch (type) {
        case ::C_SVC:
            return "C_SVC";
        case ::NU_SVC:
            return "NU_SVC";
        case ::ONE_CLASS:
            return "ONE_CLASS";
        case  ::EPSILON_SVR:
            return "EPSILON_SVR";
        case ::NU_SVR:
            return "NU_SVR";
        default:
            return "unknon type";
    }
}

const char* kernel2string(int kernel)
{
    switch (kernel) {
        case ::LINEAR:
            return "LINEAR";
        case  ::POLY:
            return "POLY";
        case  ::RBF:
            return "RBF";
        case  ::SIGMOID:
            return "SIGMOID";
        case  ::PRECOMPUTED:
            return "PRECOMPUTED";
        default:
            return "unknown kernel type";
    }
}

std::ostream& operator << (std::ostream& os, const svm_parameter& p)
{
    return os<<"gamma: "<<p.gamma<<" coast: "<<p.C<<", type "
        <<type2string(p.svm_type)<<", kernel "<<kernel2string(p.kernel_type)
        <<", coefo "<<p.coef0<<" eps "<<p.eps<<" nr_weight "<<p.nr_weight<<" nu "<<p.nu
        <<" degree "<<p.degree<<" p "<<p.p<<
        " shinking "<<std::boolalpha<<(bool)p.shrinking
        <<", pobbility support "<<std::boolalpha<<(bool)p.probability;
}

