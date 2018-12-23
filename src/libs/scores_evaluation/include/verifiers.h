#pragma once
#include "libs/experiments/ROC_results.h"
#include "libs/experiments/ks_results.h"
#include "libs/experiments/mse_results.h"
#include "libs/experiments/cm_results.h"
#include <string>
#include <iosfwd>

struct null_output{};

// These are the alogrithms that we are using to verify the testing result
// They are run on the data that we get after running the testing for a given
// model. note that we don't need to know what was the model that generate the data
// these are "tags" that we would use to overload the action for verification
struct AUC_ROC
{
    using result_type = exprs::ROC_results;
};
std::ostream& operator << (std::ostream& os, const AUC_ROC&);
std::string to_string(const AUC_ROC&);

struct logistic_loss
{
    using result_type = null_output;
};
std::ostream& operator << (std::ostream& os, const logistic_loss&);
std::string to_string(const logistic_loss&);

struct kalmogorov_smirnov_test
{
    using result_type = exprs::ks::result_type;
};
std::ostream& operator << (std::ostream& os, const kalmogorov_smirnov_test&);
std::string to_string(const kalmogorov_smirnov_test&);

struct MSE_test
{
    using result_type = exprs::mse_results;
};
std::ostream& operator << (std::ostream& os, const MSE_test&);
std::string to_string(const MSE_test&);

// for the case where the action is not supported by the given model
struct null_verifier
{
    using result_type = null_output;
};
std::ostream& operator << (std::ostream& os, null_verifier);
std::string to_string(const null_verifier&);

struct confusion_matric_test
{
    using result_type = exprs::cm_results;
};
std::ostream& operator << (std::ostream& os, const confusion_matric_test& cm);
std::string to_string(const confusion_matric_test& mc);

