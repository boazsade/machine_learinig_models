#include "libs/regression_models_actions/regressionsmodels_task.h"
#include "logging.h"
#include <iostream>
#include <tuple>

namespace regression
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

}   // end of local namespace

MLP_train::result_type
    preform_action(const MLP_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"MLP regression train called with class count "<<actions::class_count(input);
    return model.execute(actions::samples_data(input), actions::labels_data(input),
                        actions::class_count(input));
}

SVM_train::result_type
    preform_action(const SVM_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"SVM regression train called with class count "<<actions::class_count(input);;
    return model.execute(actions::samples_data(input), actions::labels_data(input),
                        actions::class_count(input));
}

logistic_regression_train::result_type
    preform_action(const logistic_regression_train& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"logistic regration regression train called with class count "<<actions::class_count(input);;;
    return model.execute(actions::samples_data(input), actions::labels_data(input),
                        actions::class_count(input));
}

///////////////////////////////////////////////////////////////////////////////
MLP_train_sv::result_type
    preform_action(const MLP_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"MLP regression train called";
    return model.execute(actions::samples_data(input), 
                actions::labels_data(input),
                actions::class_count(input));
}

SVM_train_sv::result_type
    preform_action(const SVM_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"SVM regression train called";
    return model.execute(actions::samples_data(input), 
                actions::labels_data(input),
                actions::class_count(input));
}

logistic_regression_train_sv::result_type
    preform_action(const logistic_regression_train_sv& model, 
            const actions::extend_train_input& input)
{
    LOG_DEBUG<<"logistic regration regression train called";
    return model.execute(actions::samples_data(input), 
                    actions::labels_data(input),
                    actions::class_count(input));
}

///////////////////////////////////////////////////////////////////////////////
MLP_test::result_type
    preform_action(const MLP_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"MLP regression test called";
    return model.execute(input);
}

SVM_test::result_type
    preform_action(const SVM_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"SVM regression test called";
    return model.execute(input);
}

logistic_regression_test::result_type
    preform_action(const logistic_regression_test& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"logistic regration regression test called";
    return model.execute(input);
}

///////////////////////////////////////////////////////////////////////////////
MLP_test_sv::result_type
    preform_action(const MLP_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"MLP regression test called";
    return run_test_with_save(model, input);
}

SVM_test_sv::result_type
    preform_action(const SVM_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"SVM regression test called";
    return run_test_with_save(model, input);
}

logistic_regression_test_sv::result_type
    preform_action(const logistic_regression_test_sv& model, 
            const actions::testing_input& input)
{
    LOG_DEBUG<<"logistic regration regression test called";
    return run_test_with_save(model, input);
}

///////////////////////////////////////////////////////////////////////////////
//
MLP_save::result_type
    preform_action(const MLP_save& model, 
            const MLP_save::input_type& input)
{
    LOG_DEBUG<<"MLP save called";
    return model.execute(input);
}


logistic_regression_save::result_type
    preform_action(const logistic_regression_save& model, 
            const logistic_regression_save::input_type& input)
{
    LOG_DEBUG<<"logistic regration regression save called";
    return model.execute(input);
}


SVM_save::result_type
    preform_action(const SVM_save& model, 
            const SVM_save::input_type& input)
{
    LOG_DEBUG<<"SVM save called";
    return model.execute(input);
}


///////////////////////////////////////////////////////////////////////////////

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

std::ostream& operator << (std::ostream& os,  const MLP_test& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os,  const MLP_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_train& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const SVM_test& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_test& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const MLP_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const logistic_regression_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const SVM_save& d)
{
    return display(os, d);
}

std::ostream& operator << (std::ostream& os, const SVM_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const MLP_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const MLP_train_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const logistic_regression_test_sv& d)
{
    return display(os, d);
}
std::ostream& operator << (std::ostream& os, const SVM_test_sv& d)
{
    return display(os, d);
}
}   // end of namespae regression

