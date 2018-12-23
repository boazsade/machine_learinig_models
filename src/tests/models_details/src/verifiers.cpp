#include "apps/run_experiments/verifiers.h"
#include <iostream>
std::ostream& operator << (std::ostream& os, const AUC_ROC&)
{
    return os<<"AUC_ROC verification";
}

std::ostream& operator << (std::ostream& os, const logistic_loss&)
{
    return os<<"logistic_loss verification";
}

std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_test&)
{
    return os<<"kalmogorov_smirnov_test verification";
}



