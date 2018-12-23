#include "apps/run_experiments/classifers_models.h"
#include <iostream>

namespace classifier
{

namespace
{
    template<typename Model>
    typename Model::model_type 
    train_model(Model tm,
            const training_data& data, 
            const class_data& labels)
    {
        return tm.train(data, labels);
    }

    template<typename Model>
    bool test_model(Model wrapper, 
            typename Model::model_type model,
            const testing_data& data, testing_result& res)
    {
        return wrapper.test(model, data, res);
    }

    struct null_model
    {
        using model_type = undefined_model;
        model_type train(const training_data&, const class_data&) const
        {
            return model_type{};
        }

        bool test(model_type ,
                const testing_data&, testing_result&) const
        {
            return false;
        }
    };
}   // end of local namespace

SVM::model_type SVM::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(support_vector_machine{}, data, labels);
}

bool SVM::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(support_vector_machine{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const SVM& )
{
    return os<<"SVM";
}

EM::model_type EM::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool EM::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const EM& )
{
    return os<<"EM";
}

normal_bayes::model_type normal_bayes::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool normal_bayes::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const normal_bayes&)
{
    return os<<"normal_bayes";
}

k_nearest::model_type k_nearest::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool k_nearest::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const k_nearest& )
{
    return os<<"k-nearset";
}

random_tree::model_type random_tree::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool random_tree::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const random_tree& )
{
    return os<<"random-tree";
}

logistic_regration::model_type logistic_regration::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool logistic_regration::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const logistic_regration& )
{
    return os<<"logistic-regration";
}

boosting::model_type boosting::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool boosting::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const boosting& )
{
    return os<<"boosting";
}

decision_tree::model_type decision_tree::train(const training_data& data,
        const class_data& labels) const
{
    return train_model(null_model{}, data, labels);
}

bool decision_tree::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    return test_model(null_model{}, model, data, res);
}

std::ostream& operator << (std::ostream& os, const decision_tree& )
{
    return os<<"decision-tree";
}

}   // end of namespace classifier

