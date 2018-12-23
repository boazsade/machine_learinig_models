#pragma once
#include "type_args.h"
#include <iostream>

namespace mlmodels
{
namespace svm
{
std::ostream& operator << (std::ostream& os, const sv_train_args& pa)
{
    return os<<"SV cost "<<pa.cost;
}

std::ostream& operator <<(std::ostream& os, const c_svm_train_args& csvm)
{
    return os<<static_cast<const sv_train_args&>(csvm)<<" weight count";
}

std::ostream& operator <<(std::ostream& os, const nu_train_args& csvm)
{
    return os<<"NU "<<csvm.nu;
}

std::ostream& operator <<(std::ostream& os, const svr_args& csvm)
{
    return os<<static_cast<const nu_train_args&>(csvm)<<" "<<static_cast<const sv_train_args>(csvm);
}

std::ostream& operator <<(std::ostream& os, const epsilon_args& ea)
{
    return os<<static_cast<const sv_train_args&>(ea)<<" epsilon "<<ea.epsilon;
}

}   // end of namesapce svm
}   // end of mlmodels
