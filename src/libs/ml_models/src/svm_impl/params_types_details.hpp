#pragma once
#include "kernel_args.h"
#include "training_args.h"
#include "type_args.h"
#include "libs/ml_models/svm_types.h"
#include <iosfwd>

namespace mlmodels
{
namespace svm
{
namespace detail
{

template<type_t Type, kernel_type Kernel>
struct params_base
{
    static const constexpr type_t type = Type;
    static const constexpr kernel_type kernel = Kernel;
};

template<type_t Type>
struct RBF_params : params_base<Type, RBF>,
                    rbf_args

{
    //using rbf_args::rbf_args;
    constexpr RBF_params() = default;
    constexpr RBF_params(const rbf_args& arg) :
                rbf_args{arg}
    {
    }
};

template<type_t Type>
struct sig_params : params_base<Type, SIGMOID>,
                    sig_args
{
    using sig_args::sig_args;
};

template<type_t Type>
struct poly_params : params_base<Type, POLY>,
                poly_args
{
    using poly_args::poly_args;
};

template<type_t Type>
struct RBF_train_params : RBF_params<Type>,
                   base_train_params 
                        

{
    constexpr RBF_train_params() = default;
    constexpr RBF_train_params(const rbf_args& rbf,
                            const base_train_params& other) :  
                    RBF_params<Type>{rbf}, base_train_params{other}
    {
    }
};

template<type_t Type>
struct sig_train_params : sig_params<Type>,
                    base_train_params
{
    constexpr sig_train_params() = default;
    constexpr sig_train_params(const sig_params<Type>& rbf,
                            const base_train_params& other) :  
                    sig_params<Type>{rbf}, base_train_params{other}
    {
    }
};

template<type_t Type>
struct poly_train_params : poly_params<Type>,
                base_train_params
{
    constexpr poly_train_params() = default;
    constexpr poly_train_params(const poly_params<Type>& rbf,
                            const base_train_params& other) :  
                    poly_params<Type>{rbf}, base_train_params{other}
    {
    }
};

template<type_t Type, kernel_type Kernel>
struct default_train_params : params_base<Type, Kernel>, 
                            base_train_params
{
    using base_train_params::base_train_params;
};

}   // end of namespace detail

///////////////////////////////////////////////////////////////////////////////
//

///////////////////////////////////////////////////////////////////////////////
//
}   // end of namesapce svm
}   // end of mlmodels

