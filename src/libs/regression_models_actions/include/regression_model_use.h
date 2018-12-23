#pragma once
#include "regression_models.h"
#include "libs/model_datatypes/model_interface_details.h"

namespace regression
{

// operations for predictions
struct MLP_predict : detail::predict_model<MLP_model>
{
    using base_t = detail::predict_model<MLP_model>;
    using base_t::base_t;
};

struct SVM_predict : detail::predict_model<SVM>
{
    using base_t = detail::predict_model<SVM>;
    using base_t::base_t;
};

struct logistic_regression_predict : detail::predict_model<logistic_regration>
{
    using base_t = detail::predict_model<logistic_regration>;
    using base_t::base_t;
};

// operations for loading

struct MLP_load : 
    detail::loading_model<MLP_model, MLP_predict>
{
    using base_t = detail::loading_model<MLP_model, MLP_predict>;
    using base_t::base_t;
};


struct SVM_load : 
    detail::loading_model<SVM, SVM_predict>
{
    using base_t = detail::loading_model<SVM, SVM_predict>;
    using base_t::base_t;
};

struct logistic_regression_load : 
    detail::loading_model<logistic_regration, logistic_regression_predict>
{
    using base_t = 
        detail::loading_model<logistic_regration, logistic_regression_predict>;
    using base_t::base_t;
};


}   // end of namesace regression

