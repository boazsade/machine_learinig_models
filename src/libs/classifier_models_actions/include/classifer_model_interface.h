#pragma once
#include "classifers_models.h"
#include "libs/model_datatypes/model_interface_details.h"
#include <tuple>

namespace classifier
{

struct SVM_save : detail::saving_model<SVM>
{
    using base_t = detail::saving_model<SVM>;
    using base_t::base_t;
};

struct EM_save : detail::saving_model<EM>
{
    using base_t = detail::saving_model<EM>;
    using base_t::base_t;
};

struct k_nearest_save : detail::saving_model<k_nearest>
{
    using base_t = detail::saving_model<k_nearest>;
    using base_t::base_t;
};

struct random_tree_save : detail::saving_model<random_tree>
{
    using base_t = detail::saving_model<random_tree>;
    using base_t::base_t;
};

struct logistic_regression_save : detail::saving_model<logistic_regration>
{
    using base_t = detail::saving_model<logistic_regration>;
    using base_t::base_t;
};

struct boosting_save : detail::saving_model<boosting_model>
{
    using base_t = detail::saving_model<boosting_model>;
    using base_t::base_t;
};

struct decision_tree_save : detail::saving_model<decision_tree>
{
    using base_t = detail::saving_model<decision_tree>;
    using base_t::base_t;
};

struct normal_bayes_save : detail::saving_model<normal_bayes>
{
    using base_t = detail::saving_model<normal_bayes>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
//
struct SVM_test : detail::testing_model<SVM>
{
    using base_t = detail::testing_model<SVM>;
    using base_t::base_t;
};

struct EM_test : detail::testing_model<EM>
{
    using base_t = detail::testing_model<EM>;
    using base_t::base_t;
};

struct k_nearest_test : detail::testing_model<k_nearest>
{
    using base_t = detail::testing_model<k_nearest>;
    using base_t::base_t;
};

struct random_tree_test : detail::testing_model<random_tree>
{
    using base_t = detail::testing_model<random_tree>;
    using base_t::base_t;
};

struct logistic_regression_test : detail::testing_model<logistic_regration>
{
    using base_t = detail::testing_model<logistic_regration>;
    using base_t::base_t;
};

struct boosting_test : detail::testing_model<boosting_model>
{
    using base_t = detail::testing_model<boosting_model>;
    using base_t::base_t;
};

struct decision_tree_test : detail::testing_model<decision_tree>
{
    using base_t = detail::testing_model<decision_tree>;
    using base_t::base_t;
};

struct normal_bayes_test : detail::testing_model<normal_bayes>
{
    using base_t = detail::testing_model<normal_bayes>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
// overloaded case, when we need to return the function that would
// allow for saving the model. this require that we would
// return not just results but also the function that do the saving
struct SVM_test_sv : SVM_test
{
    using base_t = SVM_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   SVM_save>; 
};

struct EM_test_sv : EM_test
{
    using base_t = EM_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   EM_save>;
};

struct k_nearest_test_sv : k_nearest_test
{
    using base_t = k_nearest_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   k_nearest_save>; 
};

struct random_tree_test_sv : random_tree_test
{
    using base_t = random_tree_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   random_tree_save>; 
};

struct logistic_regression_test_sv : logistic_regression_test
{
    using base_t = logistic_regression_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   logistic_regression_save>; 
};

struct boosting_test_sv : boosting_test
{
    using base_t = boosting_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   boosting_save>; 
};

struct decision_tree_test_sv : decision_tree_test
{
    using base_t = decision_tree_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   decision_tree_save>; 
};

struct normal_bayes_test_sv : normal_bayes_test
{
    using base_t = normal_bayes_test;
    using base_t::base_t;
    using result_type = std::tuple<base_t::result_type, 
                                   normal_bayes_save>; 
};

///////////////////////////////////////////////////////////////////////////////

struct SVM_train : 
    detail::training_impl<SVM, SVM_test>
{
    using base_t = detail::training_impl<SVM, SVM_test>;
    using base_t::base_t;
};

struct EM_train : 
    detail::training_impl<EM, EM_test>
{
    using base_t = detail::training_impl<EM, EM_test>;
    using base_t::base_t;
};

struct normal_bayes_train : 
    detail::training_impl<normal_bayes, normal_bayes_test>
{
    using base_t = detail::training_impl<normal_bayes, normal_bayes_test>;
    using base_t::base_t;
};

struct k_nearest_train : 
    detail::training_impl<k_nearest, k_nearest_test>
{
    using base_t = detail::training_impl<k_nearest, k_nearest_test>;
    using base_t::base_t;
};

struct random_tree_train : 
    detail::training_impl<random_tree, random_tree_test>
{
    using base_t = detail::training_impl<random_tree, random_tree_test>;
    using base_t::base_t;
};

struct logistic_regression_train : 
    detail::training_impl<logistic_regration, logistic_regression_test>
{
    using base_t = 
        detail::training_impl<logistic_regration, logistic_regression_test>;
    using base_t::base_t;
};

struct boosting_train : 
    detail::training_impl<boosting_model, boosting_test>
{
    using base_t = detail::training_impl<boosting_model, boosting_test>;
    using base_t::base_t;
};

struct decision_tree_train : 
    detail::training_impl<decision_tree, decision_tree_test>
{
    using base_t = detail::training_impl<decision_tree, decision_tree_test>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
// note, this would allow to return a model that we can later save. since
// the operation that we doing is derived from the first step, which is
// the building of the model - training we have to start here

struct SVM_train_sv : 
    detail::training_impl<SVM, SVM_test_sv>
{
    using base_t = detail::training_impl<SVM, SVM_test_sv>;
    using base_t::base_t;
};

struct EM_train_sv : 
    detail::training_impl<EM, EM_test_sv>
{
    using base_t = detail::training_impl<EM, EM_test_sv>;
    using base_t::base_t;
};

struct normal_bayes_train_sv : 
    detail::training_impl<normal_bayes, normal_bayes_test_sv>
{
    using base_t = detail::training_impl<normal_bayes, normal_bayes_test_sv>;
    using base_t::base_t;
};

struct k_nearest_train_sv : 
    detail::training_impl<k_nearest, k_nearest_test_sv>
{
    using base_t = detail::training_impl<k_nearest, k_nearest_test_sv>;
    using base_t::base_t;
};

struct random_tree_train_sv : 
    detail::training_impl<random_tree, random_tree_test_sv>
{
    using base_t = detail::training_impl<random_tree, random_tree_test_sv>;
    using base_t::base_t;
};

struct logistic_regression_train_sv : 
    detail::training_impl<logistic_regration, logistic_regression_test_sv>
{
    using base_t = 
        detail::training_impl<logistic_regration, logistic_regression_test_sv>;
    using base_t::base_t;
};

struct boosting_train_sv : 
    detail::training_impl<boosting_model, boosting_test_sv>
{
    using base_t = detail::training_impl<boosting_model, boosting_test_sv>;
    using base_t::base_t;
};

struct decision_tree_train_sv : 
    detail::training_impl<decision_tree, decision_tree_test_sv>
{
    using base_t = detail::training_impl<decision_tree, decision_tree_test_sv>;
    using base_t::base_t;
};

}   // end of namespace classifier


