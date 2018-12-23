#pragma once
#include "training_args.h"
#include <iostream>

namespace mlmodels
{
namespace svm
{
std::ostream& operator << (std::ostream& os, const base_train_params& btp)
{
    return os<<" training args cache size "<<btp.cache_size<<", stop crtira "<<btp.stop_crit<<", shrinking "
        <<std::boolalpha<<btp.shrinking<<", do probability estimates "<<std::boolalpha<<btp.probability;
}

}   // end of namesapce svm
}   // end of mlmodels
