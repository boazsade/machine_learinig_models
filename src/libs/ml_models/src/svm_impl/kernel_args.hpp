#pragma once
#include "kernel_args.h"
#include <iostream>

namespace mlmodels
{
namespace svm
{
std::ostream& operator << (std::ostream& os, const rbf_args& pa)
{
    return os<<"RBF gamma "<<pa.gamma;
}

std::ostream& operator << (std::ostream& os, const sig_args& pa)
{
    return os<<static_cast<const rbf_args&>(pa)<<", SIGMOID coef0 "<<pa.coef0;
}

std::ostream& operator << (std::ostream& os, const poly_args& pa)
{
    return os<<static_cast<const sig_args&>(pa)<<" POLY degree "<<pa.degree;
}

}   // end of namesapce svm
}   // end of mlmodels
