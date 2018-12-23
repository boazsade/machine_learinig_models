#pragma once
#include "classifers_models.h"

namespace classifier
{

namespace detail
{

template<typename Impl>
struct testing_model
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = typename Impl::testing_input;
    using result_type = typename Impl::test_output;

    testing_model(model_type m) :
        model{std::move(m)}
    {
    }

    result_type execute(const input_type& input) const
    {
        Impl mr;
        result_type result;
        if (mr.test(model, input, result)) {
            return result;
        } else {
            static const result_type err = result_type{};
            return err;
        }
    }

private:
    model_type model;
};


template<typename Impl, typename TrainRes>
struct training_impl
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = typename Impl::training_input;
    using result_type = TrainRes;

    training_impl() = default;

    result_type execute(const input_type& input,
            const class_data& labels) const
    {
        Impl mr;
        auto model = mr.train(input, labels);
        return result_type{std::move(model)};
    }

private:
};

}   // end of namesace detail



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

struct logistic_regration_test : detail::testing_model<logistic_regration>
{
    using base_t = detail::testing_model<logistic_regration>;
    using base_t::base_t;
};

struct boosting_test : detail::testing_model<boosting>
{
    using base_t = detail::testing_model<boosting>;
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

struct logistic_regration_train : 
    detail::training_impl<logistic_regration, logistic_regration_test>
{
    using base_t = detail::training_impl<logistic_regration, logistic_regration_test>;
    using base_t::base_t;
};

struct boosting_train : 
    detail::training_impl<boosting, boosting_test>
{
    using base_t = detail::training_impl<boosting, boosting_test>;
    using base_t::base_t;
};

struct decision_tree_train : 
    detail::training_impl<decision_tree, decision_tree_test>
{
    using base_t = detail::training_impl<decision_tree, decision_tree_test>;
    using base_t::base_t;
};

///////////////////////////////////////////////////////////////////////////////
//

#if 0
template<typename T, typename R, typename A> 
R preform_action(const test_model<T>&, const A&);

template<typename T, typename R, typename A> 
R preform_action(const train_model<T>&, const A&);
#endif
}   // end of classifier

