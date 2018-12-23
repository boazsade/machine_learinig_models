#include "libs/scores_evaluation/verifiers.h"
#include <iostream>
std::ostream& operator << (std::ostream& os, const AUC_ROC& v)
{
    return os<<to_string(v);
}

std::ostream& operator << (std::ostream& os, const logistic_loss& v)
{
    return os<<to_string(v);
}

std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_test& v)
{
    return os<<to_string(v);
}

std::ostream& operator << (std::ostream& os, const MSE_test& v)
{
    return os<<to_string(v);
}

std::ostream& operator << (std::ostream& os, null_verifier v)
{
    return os<<to_string(v);
}

std::string to_string(const logistic_loss&)
{
    using namespace std::string_literals;
    return "logistic_loss"s;
}

std::string to_string(const MSE_test&)
{
    using namespace std::string_literals;
    return "mse"s;
}

std::string to_string(const null_verifier&)
{
    using namespace std::string_literals;
    return "na"s;
}


std::string to_string(const kalmogorov_smirnov_test&)
{
    using namespace std::string_literals;
    return "ks-test"s;
}

std::string to_string(const AUC_ROC&)
{
    using namespace std::string_literals;
    return "auc-roc";
}

std::ostream& operator << (std::ostream& os, const confusion_matric_test& cm)
{
    return os<<to_string(cm);
}

std::string to_string(const confusion_matric_test& )
{
    using namespace std::string_literals;
    return "cm"s;
}
