#pragma once
#include "classifers_models.h"
#include "libs/model_datatypes/model_interface_details.h"

namespace classifier
{

///////////////////////////////////////////////////////////////////////////////
// for prediction - we are calling the prediction function that allow getting
// results for a given input - this is used with an exiting model that
// was either loaded or loaded for model that was saved after load
struct SVM_predict : detail::predict_model<SVM>
{
    using base_t = detail::predict_model<SVM>;
    using base_t::base_t;
};

struct EM_predict : detail::predict_model<EM>
{
    using base_t = detail::predict_model<EM>;
    using base_t::base_t;
};

struct k_nearest_predict : detail::predict_model<k_nearest>
{
    using base_t = detail::predict_model<k_nearest>;
    using base_t::base_t;
};

struct random_tree_predict : detail::predict_model<random_tree>
{
    using base_t = detail::predict_model<random_tree>;
    using base_t::base_t;
};

struct logistic_regression_predict : detail::predict_model<logistic_regration>
{
    using base_t = detail::predict_model<logistic_regration>;
    using base_t::base_t;
};

struct boosting_predict : detail::predict_model<boosting_model>
{
    using base_t = detail::predict_model<boosting_model>;
    using base_t::base_t;
};

struct decision_tree_predict : detail::predict_model<decision_tree>
{
    using base_t = detail::predict_model<decision_tree>;
    using base_t::base_t;
};

struct normal_bayes_predict : detail::predict_model<normal_bayes>
{
    using base_t = detail::predict_model<normal_bayes>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
// for loading - this replace the interface for load - we are assuming that 
// we have it already

struct SVM_load : 
        detail::loading_model<SVM, SVM_predict>
{
    using base_t = detail::loading_model<SVM, SVM_predict>;
    using base_t::base_t;
};

struct EM_load : 
    detail::loading_model<EM, EM_predict>
{
    using base_t = detail::loading_model<EM, EM_predict>;
    using base_t::base_t;
};

struct normal_bayes_load : 
    detail::loading_model<normal_bayes, normal_bayes_predict>
{
    using base_t = detail::loading_model<normal_bayes, normal_bayes_predict>;
    using base_t::base_t;
};

struct k_nearest_load : 
    detail::loading_model<k_nearest, k_nearest_predict>
{
    using base_t = detail::loading_model<k_nearest, k_nearest_predict>;
    using base_t::base_t;
};

struct random_tree_load : 
    detail::loading_model<random_tree, random_tree_predict>
{
    using base_t = detail::loading_model<random_tree, random_tree_predict>;
    using base_t::base_t;
};

struct logistic_regression_load : 
    detail::loading_model<logistic_regration, logistic_regression_predict>
{
    using base_t = 
        detail::loading_model<logistic_regration, logistic_regression_predict>;
    using base_t::base_t;
};

struct boosting_load : 
    detail::loading_model<boosting_model, boosting_predict>
{
    using base_t = detail::loading_model<boosting_model, boosting_predict>;
    using base_t::base_t;
};

struct decision_tree_load : 
    detail::loading_model<decision_tree, decision_tree_predict>
{
    using base_t = detail::loading_model<decision_tree, decision_tree_predict>;
    using base_t::base_t;
};

}   // end of namespace classifier


