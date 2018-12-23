#include "apps/run_experiments/classifers_models.h"
#include "logging.h"
#include <iostream>
#include <fstream>
//#define DUMP_MODEL_FILE
namespace classifier
{

namespace
{
#ifdef DUMP_MODEL_FILE
    template<typename T>
    void dump_model(T model)
    {
        save_model(model);
    }
#else
    template<typename T>
    void dump_model(T) {}
#endif 
#if DUMP_DATA_FOR_MODEL
    void dump_inputs(const model_base::training_input& td, 
            std::ostream& to)
    {
        to<<"data matrix "<<rows(td)<<" X "<<columns(td)<<"\n---------------------------------------\n";
        using index_type = model_base::training_input::index;
        for(index_type i = 0; i != (index_type)rows(td); ++i) {
            for(index_type j = 0; j != (index_type)columns(td); ++j) {
                to<<td[i][j]<<" ";
            }
            to<<std::endl;
        }
    }
    void dump_data(const model_base::training_input& td,
            const classes_data& cd)
    {
        //std::ofstream log{"log.txt"};
        std::cout<<"train data\n";
        dump_inputs(td, std::cout);
        std::cout<<"\nlabels ("<<cd.size()<<")\n-------------------------\n";
        std::copy(std::begin(cd), std::end(cd), std::ostream_iterator<classes_data::value_type>(std::cout, " "));
        std::cout<<std::endl;
    }
#else
    void dump_data(const model_base::training_input& ,
            const classes_data& )
    {
    }
#endif  // DUMP_DATA_FOR_MODEL
#if DUMP_ROW_MODEL_OUTPUT
    void dump_inputs(const model_base::training_input& td, 
            const classifier::class_data& ro,
            std::ostream& to)
    {
        to<<"data matrix "<<rows(td)<<" X "<<columns(td)<<", predicions: "<<ro.size()<<"\n---------------------------------------\n";
        if (ro.size() != rows(td)) {
            return;
        }
        using index_type = model_base::training_input::index;
        index_type i = 0; 
        for(;i != (index_type)rows(td); ++i) {
            for(index_type j = 0; j != (index_type)columns(td); ++j) {
                to<<td[i][j]<<" ";
            }
            to<<ro[i]<<std::endl;
        }
    }
    void dump_data(const model_base::training_input& td,
            const support_vector_machine::row_output& cd)
    {
        dump_inputs(td, cd, std::cout);
        std::cout<<std::endl;

    }
#else
    void dump_data(const model_base::training_input& ,
                          const support_vector_machine::row_output&)
    {
    }
#endif  // DUMP_ROW_MODEL_OUTPUT


    template<typename Model>
    typename Model::model_type 
    train_model(Model tm,
            const model_base::training_input& data, 
            const classes_data& labels)
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
        model_type train(const model_base::training_input&, const classes_data&) const
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

SVM::model_type SVM::train(const training_input& data,
        const classes_data& labels) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return SVM::model_type{};
    } else {
        LOG_INFO<<"running training for SVM with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        dump_data(data, labels);
        return train_model(support_vector_machine{}, data, labels);
    }
}

bool SVM::test(model_type model, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train";
        return false;
    } else {
        LOG_INFO<<"running test for SVM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        //dump_model(model);
        support_vector_machine svm;
        auto row = svm.test(model, data);
        dump_data(data, row);
        return test_model(support_vector_machine{}, model, data, res);
    }
}

std::ostream& operator << (std::ostream& os, const SVM& )
{
    return os<<"SVM";
}

EM::args::args(int cc, int mic, float tc,
        covariance_matrices_t ms) :
            cluster_count(cc), min_in_cluster(mic),
            termination_crtiria(tc), mat_shape(ms)
{
}

EM::model_type EM::train(const training_input& data,
        const classes_data& labels, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid data for EM training - empty";
        return EM::model_type{};
    } else {
        LOG_INFO<<"running test for EM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        expectation_maximization trainer;
        return trainer.train(data, labels, 
                a.cluster_count, a.min_in_cluster, 
                a.termination_crtiria, a.mat_shape
            );
    }
}

bool EM::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid data for EM testing - empty";
        return false;
    } else {
        LOG_INFO<<"running test for EM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        return test_model(expectation_maximization{}, model, data, res);
    }
}

std::ostream& operator << (std::ostream& os, const EM& )
{
    return os<<"EM";
}

normal_bayes::model_type normal_bayes::train(const training_input& data,
        const classes_data& labels) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model_type{};
    } else {
        LOG_INFO<<"running training for SVM with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        dump_data(data, labels);
        return train_model(normal_bayes_classifier{}, data, labels);
    }
}

bool normal_bayes::test(model_type model, const testing_data& data,
        testing_result& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train";
        return false;
    } else {
        LOG_INFO<<"running test for SVM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        //dump_model(model);
        normal_bayes_classifier nb;
        auto row = nb.test(model, data);
        dump_data(data, row);
        return test_model(normal_bayes_classifier{}, model, data, res);
    }
}

std::ostream& operator << (std::ostream& os, const normal_bayes&)
{
    return os<<"normal_bayes";
}

k_nearest::model_type k_nearest::train(const training_input& data,
        const classes_data& labels) const
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

random_tree::model_type random_tree::train(const training_input& data,
        const classes_data& labels) const
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

logistic_regration::model_type logistic_regration::train(const training_input& data,
        const classes_data& labels) const
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

boosting::model_type boosting::train(const training_input& data,
        const classes_data& labels) const
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

decision_tree::model_type decision_tree::train(const training_input& data,
        const classes_data& labels) const
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

