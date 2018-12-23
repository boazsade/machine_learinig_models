#include "libs/ml_models/model_types.h"
#include <iostream>

namespace mlmodels
{


std::ostream& operator << (std::ostream& os, classifier_model)
{
    return os<<"classifier";
}

std::ostream& operator << (std::ostream& os, regression_model)
{
    return os<<"regression";
}

}   // end of namespace mlmodels

