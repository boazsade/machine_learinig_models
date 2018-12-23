#pragma once
#include "params_types_details.hpp"

namespace mlmodels
{
namespace svm
{

namespace detail
{

template<typename B1, typename B2>
struct reg_base_types : B1, B2
{
    using base_1 = B1;
    using base_2 = B2;

    constexpr reg_base_types() = default;
    constexpr reg_base_types(const base_1& a1, 
                    const base_2& a2) :
            B1{a1}, B2{a2}
    {
    }
};

}   // end of detail
namespace regression
{
///////////////////////////////////////////////////////////////////////////////
// none trainig types
using nu_linear = detail::params_base<NU_SVR, LINEAR>;
using nu_rbf = detail::RBF_params<NU_SVR>;
using nu_poly = detail::poly_params<NU_SVR>;
using nu_sig = detail::sig_params<NU_SVR>;
using epsilon_linear = detail::params_base<EPS_SVR, LINEAR>;
using epsilon_rbf = detail::RBF_params<EPS_SVR>;
using epsilon_poly = detail::poly_params<EPS_SVR>;
using epsilon_sig = detail::sig_params<EPS_SVR>;


///////////////////////////////////////////////////////////////////////////////
//  training types
//  LINEAR
struct epsilon_linear_train : 
                    detail::reg_base_types<detail::default_train_params<EPS_SVR, LINEAR>, epsilon_args>
{
    using detail::reg_base_types<detail::default_train_params<EPS_SVR, LINEAR>, epsilon_args>::reg_base_types;
};

struct nu_linear_train : 
                    detail::reg_base_types<detail::default_train_params<NU_SVR, LINEAR>, svr_args>
{
    using detail::reg_base_types<detail::default_train_params<NU_SVR, LINEAR>, svr_args>::reg_base_types;
};

//  RBF
struct epsilon_rbf_train : 
                    detail::reg_base_types<detail::RBF_train_params<EPS_SVR>, epsilon_args>
{
    using detail::reg_base_types<detail::RBF_train_params<EPS_SVR>, epsilon_args>::reg_base_types;
};

struct nu_rbf_train : 
                    detail::reg_base_types<detail::RBF_train_params<NU_SVR>, svr_args>
{
    using detail::reg_base_types<detail::RBF_train_params<NU_SVR>, svr_args>::reg_base_types;

};

//  SIG
struct epsilon_sig_train : 
                    detail::reg_base_types<detail::sig_train_params<EPS_SVR>, epsilon_args>
{
    using detail::reg_base_types<detail::sig_train_params<EPS_SVR>, epsilon_args>::reg_base_types;
};

struct nu_sig_train : 
                    detail::reg_base_types<detail::sig_train_params<NU_SVR>, svr_args>
{
    using detail::reg_base_types<detail::sig_train_params<NU_SVR>, svr_args>::reg_base_types;
};

//  POLY
struct epsilon_poly_train : 
                    detail::reg_base_types<detail::poly_train_params<EPS_SVR>, epsilon_args>
{
    using detail::reg_base_types<detail::poly_train_params<EPS_SVR>, epsilon_args>::reg_base_types;
};

struct nu_poly_train : 
                    detail::reg_base_types<detail::poly_train_params<NU_SVR>, svr_args>
{
    using detail::reg_base_types<detail::poly_train_params<NU_SVR>, svr_args>::reg_base_types;
};


}   // end of namespace regression

}   // end of namesapce svm
}   // end of mlmodels

