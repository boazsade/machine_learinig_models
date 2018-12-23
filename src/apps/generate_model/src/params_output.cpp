#include "apps/generate_model/params_output.h"
#include "libs/experiments/ROC_results.h"
#include "libs/experiments/ks_results.h"
#include "libs/experiments/mse_results.h"
#include <iostream>

params_output::params_output(const AUC_ROC::result_type& auc) :
    AUC(auc)
{
}

params_output::params_output(const kalmogorov_smirnov_test::result_type& ks) : 
        KS(ks)
{
}

params_output::params_output(const MSE_test::result_type& mse) :
    MSE(mse)
{
}
   

params_output::params_output(const AUC_ROC::result_type& auc,
            const kalmogorov_smirnov_test::result_type& ks,
            const MSE_test::result_type& mse) :
    AUC(auc), KS(ks), MSE(mse)
{
}

params_output::params_output(AUC_ROC::result_type&& auc,
            kalmogorov_smirnov_test::result_type&& ks,
            MSE_test::result_type&& mse) :
    AUC(auc), KS(ks), MSE(mse)
{
}

params_output& params_output::operator = (const AUC_ROC::result_type& auc)
{
    if (!AUC) {
        AUC = auc;
    }
    return *this;
}

params_output& params_output::operator = (const kalmogorov_smirnov_test::result_type& ks)
{
    if (!KS) {
        KS = ks;
    }
    return *this;
}

params_output& params_output::operator = (const MSE_test::result_type& mse)
{
    if (!MSE) {
        MSE = mse;
    }
    return *this;
}

params_output& params_output::operator = (AUC_ROC::result_type&& auc)
{
    if (!AUC) {
        AUC = auc;
    }
    return *this;
}

params_output& params_output::operator = (kalmogorov_smirnov_test::result_type&& ks)
{
    if (!KS) {
        KS = ks;
    }
    return *this;
}

params_output& params_output::operator = (MSE_test::result_type&& mse)
{
    if (!MSE) {
        MSE = mse;
    }
    return *this;
}

params_output& params_output::operator += (const params_output& other)
{
    if (other.AUC) {
        *this = other.AUC.get();
    } 
    if (other.KS) {
        *this = other.KS.get();
    }
    if (other.MSE) {
        *this = other.MSE.get();
    }
    return *this;
}

const params_output& compare(const params_output& left,
                             const params_output& right)
{
    if (left.AUC && left.MSE && left.KS) {
        return left;
    }
    return right;
}

std::ostream& operator << (std::ostream& os, const params_output& po)
{
    os<<"output measured testing: ";
    if (po.KS) {
        os<<"{kalmogorov/smirnov: "<<po.KS.get()<<"}";
    }
    if (po.AUC) {
        os<<"{AUC: "<<po.AUC.get()<<"}";
    }
    if (po.MSE) {
        os<<"{MSE: "<<po.MSE.get()<<"}";
    }
    return os;
}

params_output operator + (const params_output& left, const params_output& right)
{
    params_output out{left};
    out += right;
    return out;
}

