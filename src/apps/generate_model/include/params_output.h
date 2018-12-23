#pragma once
#include "libs/scores_evaluation/verifiers.h"
#include <boost/optional.hpp>
#include <iosfwd>

struct params_output
{
    using auc_output = boost::optional<AUC_ROC::result_type>;
    using ks_output = boost::optional<kalmogorov_smirnov_test::result_type>;
    using mse_output = boost::optional<MSE_test::result_type>;
    
    params_output() = default;

    explicit params_output(const AUC_ROC::result_type& auc);
    explicit params_output(const kalmogorov_smirnov_test::result_type& ks);
    explicit params_output(const MSE_test::result_type& mse);
   

    params_output(const AUC_ROC::result_type& auc,
            const kalmogorov_smirnov_test::result_type& ks,
            const MSE_test::result_type& mse);

    params_output(AUC_ROC::result_type&& auc,
            kalmogorov_smirnov_test::result_type&& ks,
            MSE_test::result_type&& mse);

    params_output& operator = (const AUC_ROC::result_type& auc);
    params_output& operator = (const kalmogorov_smirnov_test::result_type& ks);
    params_output& operator = (const MSE_test::result_type& mse);
    params_output& operator = (AUC_ROC::result_type&& auc);
    params_output& operator = (kalmogorov_smirnov_test::result_type&& ks);
    params_output& operator = (MSE_test::result_type&& mse);

    params_output& operator += (const params_output& other);

    auc_output AUC;
    ks_output KS;
    mse_output MSE;
};

// this is specific to decide which has a better result in
// term of the model that we are holding
const params_output& compare(const params_output& left,
                             const params_output& right);

std::ostream& operator << (std::ostream& os, const params_output&);
params_output operator + (const params_output& left, const params_output& right);
