#pragma once
#include "regression_models.h"
#include "libs/model_datatypes/model_interface_details.h"

namespace regression
{


struct MLP_save : detail::saving_model<MLP_model>
{
    using base_t = detail::saving_model<MLP_model>;
    using base_t::base_t;
};

struct SVM_save : detail::saving_model<SVM>
{
    using base_t = detail::saving_model<SVM>;
    using base_t::base_t;
};

struct logistic_regression_save : detail::saving_model<logistic_regration>
{
    using base_t = detail::saving_model<logistic_regration>;
    using base_t::base_t;
};


///////////////////////////////////////////////////////////////////////////////
struct MLP_test : detail::testing_model<MLP_model>
{
    using base_t = detail::testing_model<MLP_model>;
    using base_t::base_t;
};

struct SVM_test : detail::testing_model<SVM>
{
    using base_t = detail::testing_model<SVM>;
    using base_t::base_t;
};

struct logistic_regression_test : detail::testing_model<logistic_regration>
{
    using base_t = detail::testing_model<logistic_regration>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
// overloaded case, when we need to return the function that would
// allow for saving the model. this require that we would
// return not just results but also the function that do the saving
struct MLP_test_sv : MLP_test
{
    using base_t = MLP_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   MLP_save>; 
};

struct SVM_test_sv : SVM_test
{
    using base_t = SVM_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   SVM_save>; 
};

struct logistic_regression_test_sv : logistic_regression_test
{
    using base_t = logistic_regression_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   logistic_regression_save>;
};

///////////////////////////////////////////////////////////////////////////////
struct MLP_train : 
    detail::training_impl<MLP_model, MLP_test>
{
    using base_t = detail::training_impl<MLP_model, MLP_test>;
    using base_t::base_t;
};


struct SVM_train : 
    detail::training_impl<SVM, SVM_test>
{
    using base_t = detail::training_impl<SVM, SVM_test>;
    using base_t::base_t;
};

struct logistic_regression_train : 
    detail::training_impl<logistic_regration, logistic_regression_test>
{
    using base_t = 
        detail::training_impl<logistic_regration, logistic_regression_test>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
// note, this would allow to return a model that we can later save. since
// the operation that we doing is derived from the first step, which is
// the building of the model - training we have to start here
struct MLP_train_sv : 
    detail::training_impl<MLP_model, MLP_test_sv>
{
    using base_t = detail::training_impl<MLP_model, MLP_test_sv>;
    using base_t::base_t;
};


struct SVM_train_sv : 
    detail::training_impl<SVM, SVM_test_sv>
{
    using base_t = detail::training_impl<SVM, SVM_test_sv>;
    using base_t::base_t;
};

struct logistic_regression_train_sv : 
    detail::training_impl<logistic_regration, logistic_regression_test_sv>
{
    using base_t = 
        detail::training_impl<logistic_regration, logistic_regression_test_sv>;
    using base_t::base_t;
};

}   // end of namespace regression
