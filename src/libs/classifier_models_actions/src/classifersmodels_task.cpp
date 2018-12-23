#include "libs/classifier_models_actions/classifersmodels_task.h"
#include "logging.h"
#include <tuple>
#include <iostream>

namespace classifier
{

namespace 
{

template<typename Tuple>
struct result_type_helper {
    using type = typename std::tuple_element<1, Tuple>::type;
};

template<typename TestModel>
typename TestModel::result_type
    run_test_with_save(const TestModel& model, 
                      const actions::testing_input& input)
{
    using target = typename result_type_helper<typename TestModel::result_type>::type;
    auto r = model.execute(input);
    return std::make_tuple(std::move(r), detail::testing_cast<target>(model));
}


}   // end of namespace 

SVM_train::result_type
    preform_action(const SVM_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"SVM classifier train called";
    return model.execute(actions::samples_data(input), 
                         actions::labels_data(input),
                         actions::class_count(input));
}


EM_train::result_type
    preform_action(const EM_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"EM train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

normal_bayes_train::result_type
    preform_action(const normal_bayes_train& model, 
            const actions::extend_train_input& input)
{
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

random_tree_train::result_type
    preform_action(const random_tree_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"random tree train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

k_nearest_train::result_type
    preform_action(const k_nearest_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"k nearest train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

logistic_regression_train::result_type
    preform_action(const logistic_regression_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"logistic classifier clsasifier train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

boosting_train::result_type
    preform_action(const boosting_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"boosting train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

decision_tree_train::result_type
    preform_action(const decision_tree_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"desition tree train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

///////////////////////////////////////////////////////////////////////////////
//
SVM_train_sv::result_type
    preform_action(const SVM_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"SVM classifier train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}


EM_train_sv::result_type
    preform_action(const EM_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"EM train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

normal_bayes_train_sv::result_type
    preform_action(const normal_bayes_train_sv& model, 
            const actions::extend_train_input& input)
{
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

random_tree_train_sv::result_type
    preform_action(const random_tree_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"random tree train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

k_nearest_train_sv::result_type
    preform_action(const k_nearest_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"k nearest train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

logistic_regression_train_sv::result_type
    preform_action(const logistic_regression_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"logistic classifier clsasifier train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

boosting_train_sv::result_type
    preform_action(const boosting_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"boosting train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

decision_tree_train_sv::result_type
    preform_action(const decision_tree_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"desition tree train called";
    return model.execute(actions::samples_data(input),
                        actions::labels_data(input),
                        actions::class_count(input));
}

///////////////////////////////////////////////////////////////////////////////
SVM_test::result_type
    preform_action(const SVM_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"SVM classifier test called";
    return model.execute(input);
}

EM_test::result_type
    preform_action(const EM_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"EM test called";
    return model.execute(input);
}

normal_bayes_test::result_type
    preform_action(const normal_bayes_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"normal bayes test called";
    return model.execute(input);
}

random_tree_test::result_type
    preform_action(const random_tree_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"random tree test called";
    return model.execute(input);
}

k_nearest_test::result_type
    preform_action(const k_nearest_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"k nearset test called";
    return model.execute(input);
}

logistic_regression_test::result_type
    preform_action(const logistic_regression_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"logistic regration classifier test called";
    return model.execute(input);
}

boosting_test::result_type
    preform_action(const boosting_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"boosting test called";
    return model.execute(input);
}

decision_tree_test::result_type
    preform_action(const decision_tree_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"decistion tree test called";
    return model.execute(input);
}

///////////////////////////////////////////////////////////////////////////////
SVM_test_sv::result_type
    preform_action(const SVM_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"SVM classifier test called";
    return run_test_with_save(model, input);
}

EM_test_sv::result_type
    preform_action(const EM_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"EM test called";
    return run_test_with_save(model, input);
}

normal_bayes_test_sv::result_type
    preform_action(const normal_bayes_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"normal bayes test called";
    return run_test_with_save(model, input);
}

random_tree_test_sv::result_type
    preform_action(const random_tree_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"random tree test called";
    return run_test_with_save(model, input);
}

k_nearest_test_sv::result_type
    preform_action(const k_nearest_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"k nearset test called";
    return run_test_with_save(model, input);
}

logistic_regression_test_sv::result_type
    preform_action(const logistic_regression_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"logistic regration classifier test called";
    return run_test_with_save(model, input);
}

boosting_test_sv::result_type
    preform_action(const boosting_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"boosting test called";
    return run_test_with_save(model, input);
}

decision_tree_test_sv::result_type
    preform_action(const decision_tree_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"decistion tree test called";
    return run_test_with_save(model, input);
}

///////////////////////////////////////////////////////////////////////////////
//

SVM_save::result_type
    preform_action(const SVM_save& model, 
            const SVM_save::input_type& input)
{
    LOG_DEBUG<<"saving SVM model";
    return model.execute(input);
}

EM_save::result_type
    preform_action(const EM_save& model, 
            const EM_save::input_type& input)
{
    LOG_DEBUG<<"saving EM model";
    return model.execute(input);
}


normal_bayes_save::result_type
    preform_action(const normal_bayes_save& model, 
            const normal_bayes_save::input_type& input)
{
    LOG_DEBUG<<"saving normal_bayes model";
    return model.execute(input);
}


random_tree_save::result_type
    preform_action(const random_tree_save& model, 
            const random_tree_save::input_type& input)
{
    LOG_DEBUG<<"saving random_tree model";
    return model.execute(input);
}


k_nearest_save::result_type
    preform_action(const k_nearest_save& model, 
            const k_nearest_save::input_type& input)
{
    LOG_DEBUG<<"saving k k_nearest model";
    return model.execute(input);
}


logistic_regression_save::result_type
    preform_action(const logistic_regression_save& model, 
            const logistic_regression_save::input_type& input)
{
    LOG_DEBUG<<"saving logistic_regression model";
    return model.execute(input);
}


boosting_save::result_type
    preform_action(const boosting_save& model, 
            const boosting_save::input_type& input)
{
    LOG_DEBUG<<"saving boosing model";
    return model.execute(input);
}


decision_tree_save::result_type
    preform_action(const decision_tree_save& model, 
            const decision_tree_save::input_type& input)
{
    LOG_DEBUG<<"saving decision_tree model";
    return model.execute(input);
}


///////////////////////////////////////////////////////////////////////////////
//
template<typename T> inline
std::ostream& display(std::ostream& os, T)
{
    typename T::model_impl model;
    return os<<model;
}

std::ostream& operator << (std::ostream& os,  const SVM_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_train& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const SVM_test& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const normal_bayes_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_test& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_test& d)
{
    return display(os, d);
}


///////////////////////////////////////////////////////////////////////
//
std::ostream& operator << (std::ostream& os,  const SVM_train_sv& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_train_sv& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const SVM_test_sv& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const normal_bayes_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const random_tree_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const k_nearest_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const boosting_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const decision_tree_test_sv& d)
{
    return display(os, d);
}


///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& os, const SVM_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const EM_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const normal_bayes_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const random_tree_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const k_nearest_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const boosting_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const decision_tree_save& d)
{
    return display(os, d);
}

}   // end of namesapce classifier
