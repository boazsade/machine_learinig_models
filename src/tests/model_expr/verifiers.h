#pragma once
#include <iosfwd>
// These are the alogrithms that we are using to verify the testing result
// They are run on the data that we get after running the testing for a given
// model. note that we don't need to know what was the model that generate the data
// these are "tags" that we would use to overload the action for verification
struct AUC_ROC
{
};
std::ostream& operator << (std::ostream& os, const AUC_ROC&);

struct logistic_loss
{
};
std::ostream& operator << (std::ostream& os, const logistic_loss&);

struct kalmogorov_smirnov_test
{
};
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_test&);
