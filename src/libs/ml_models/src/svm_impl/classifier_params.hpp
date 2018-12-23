#pragma once
#include "params_types_details.hpp"
namespace mlmodels
{
namespace svm
{

namespace classifier
{
///////////////////////////////////////////////////////////////////////////////
// params not for training!
using c_linear = detail::params_base<C_SVC, LINEAR>;
using nu_linear = detail::params_base<NU_SVC, LINEAR>;
using one_linear = detail::params_base<ONE_CLASS, LINEAR>;
using c_poly = detail::poly_params<C_SVC>;
using nu_poly_ = detail::poly_params<NU_SVC>;
using one_poly = detail::poly_params<ONE_CLASS>;
using c_rbf = detail::RBF_params<C_SVC>;
using nu_rbf = detail::RBF_params<NU_SVC>;
using one_rbf = detail::RBF_params<ONE_CLASS>;
using c_sig = detail::sig_params<C_SVC>;
using nu_sig = detail::sig_params<NU_SVC>;
using one_sig = detail::sig_params<ONE_CLASS>;

///////////////////////////////////////////////////////////////////////////////
// training
// RBF
struct c_rbf_train : detail::RBF_train_params<C_SVC>,
                    c_svm_train_args
{
    using base_1 = detail::RBF_train_params<C_SVC>;
    using base_2 = c_svm_train_args;
    constexpr c_rbf_train() = default;
    constexpr c_rbf_train(const  base_1& rbf,
                    const base_2& c) :
            base_1{rbf}, base_2{c}
    {
    }
};

struct nu_rbf_train : detail::RBF_train_params<NU_SVC>,
                      nu_train_args
{
    using base_1 = detail::RBF_train_params<NU_SVC>;
    using base_2 = nu_train_args;
    constexpr nu_rbf_train() = default;
    constexpr nu_rbf_train(const base_1& rbf,
            const base_2& a) : 
                base_1{rbf}, base_2{a}
    {
    }


};

struct one_rbf_train : nu_rbf_train {
    using nu_rbf_train::nu_rbf_train;
};
// SIG
struct c_sig_train : detail::sig_train_params<C_SVC>,
                    c_svm_train_args
{
    using base_1 = detail::sig_train_params<C_SVC>;
    using base_2 = c_svm_train_args;

    constexpr c_sig_train() = default;
    constexpr c_sig_train(const base_1& a1,
                const base_2& a2) : 
                    base_1{a1}, base_2{a2}
    {
    }
};

struct nu_sig_train : detail::sig_train_params<NU_SVC>,
                      nu_train_args
{
    using base_1 = detail::sig_train_params<NU_SVC>;
    using base_2 = nu_train_args;

    constexpr nu_sig_train() = default;
    constexpr nu_sig_train(const detail::sig_train_params<NU_SVC>& a1,
                    const nu_train_args& a2) : 
                        detail::sig_train_params<NU_SVC>{a1}, nu_train_args{a2}
    {
    }
};

struct one_sig_train : nu_sig_train {
    using nu_sig_train::nu_sig_train;
};

// POLY
struct c_poly_train : detail::poly_train_params<C_SVC>,
                    c_svm_train_args
{
    using base_1 = detail::poly_train_params<C_SVC>;
    using base_2 = c_svm_train_args;
    constexpr c_poly_train() = default;
    constexpr c_poly_train(const base_1& a1,
                    const base_2& a2) :
                       base_1{a1},
                        base_2{a2}
    {
    }

};

struct nu_poly_train : detail::poly_train_params<NU_SVC>,
                      nu_train_args
{
    using base_1 = detail::poly_train_params<NU_SVC>;
    using base_2 = nu_train_args;

    constexpr nu_poly_train() = default;
    constexpr nu_poly_train(const base_1& a1,
            const base_2& a2) : 
                base_1{a1}, base_2{a2}
    {
    }
};
struct one_poly_train : nu_poly_train {
    using nu_poly_train::nu_poly_train;
};
// LINEAR
struct c_linear_train : detail::default_train_params<C_SVC, LINEAR>,
                    c_svm_train_args
{
    using base_1 = detail::default_train_params<C_SVC, LINEAR>;
    using base_2 = c_svm_train_args;

    constexpr c_linear_train() = default;
    constexpr c_linear_train(const base_1& a1,
                const base_2& a2) : 
                    base_1{a1}, base_2{a2}
    {
    }
};

struct nu_linear_train : detail::default_train_params<NU_SVC, LINEAR>,
                    nu_train_args
{
    using base_1 = detail::default_train_params<NU_SVC, LINEAR>;
    using base_2 = nu_train_args;

    constexpr nu_linear_train() = default;
    constexpr nu_linear_train(const base_1& a1,
                const base_2& a2) : 
                    base_1{a1}, base_2{a2}
    {
    }
};
struct  one_linear_train : nu_linear_train {
    using nu_linear_train::nu_linear_train;
};

}   // end of namespace classifier
}   // end of namesapce svm
}   // end of mlmodels

