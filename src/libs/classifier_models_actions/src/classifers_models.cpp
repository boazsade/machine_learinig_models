#include "libs/classifier_models_actions/classifers_models.h"
#include "logging.h"
#include <iostream>
#include <fstream>
//#define DUMP_MODEL_FILE
namespace classifier
{

namespace
{
///////////////////////////////////////////////////////////////////////////////
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
    void dump_inputs(const model_base::training_data& td, 
            std::ostream& to)
    {
        to<<"data matrix "<<rows(td)<<" X "<<columns(td)<<"\n---------------------------------------\n";
        using index_type = model_base::training_data::index;
        for(index_type i = 0; i != (index_type)rows(td); ++i) {
            for(index_type j = 0; j != (index_type)columns(td); ++j) {
                to<<td[i][j]<<" ";
            }
            to<<std::endl;
        }
    }
    void dump_data(const model_base::training_data& td,
            const class_data& cd)
    {
        //std::ofstream log{"log.txt"};
        std::cout<<"train data\n";
        dump_inputs(td, std::cout);
        std::cout<<"\nlabels ("<<cd.size()<<")\n-------------------------\n";
        std::copy(std::begin(cd), std::end(cd), 
                std::ostream_iterator<classes_data::value_type>(std::cout, " "));
        std::cout<<std::endl;
    }
#else
#endif  // DUMP_DATA_FOR_MODEL
#if DUMP_ROW_MODEL_OUTPUT
    void dump_inputs(const model_base::training_data& td, 
            const classifier::class_data& ro,
            std::ostream& to)
    {
        to<<"data matrix "<<rows(td)<<" X "<<columns(td)<<", predicions: "<<ro.size()<<"\n---------------------------------------\n";
        if (ro.size() != rows(td)) {
            return;
        }
        using index_type = model_base::training_data::index;
        index_type i = 0; 
        for(;i != (index_type)rows(td); ++i) {
            for(index_type j = 0; j != (index_type)columns(td); ++j) {
                to<<td[i][j]<<" ";
            }
            to<<ro[i]<<std::endl;
        }
    }
    void dump_data(const model_base::training_data& td,
            const support_vector_machine::row_output& cd)
    {
        dump_inputs(td, cd, std::cout);
        std::cout<<std::endl;

    }
#else
#endif  // DUMP_ROW_MODEL_OUTPUT
///////////////////////////////////////////////////////////////////////////////

    template<typename Model, typename Args>
    typename Model::model_type 
    train_model(Model tm,
            const model_base::training_data& data, 
            const model_base::labels_type& labels, const Args& a)
    {
        return tm.train(data, labels, a);
    }

    template<typename Model>
    bool do_predication(Model wrapper, 
            typename Model::model_type model,
            const model_base::testing_input& data, model_base::test_output& res)
    {
        return wrapper.predict(model, data, res);
    }

    template<typename Model, typename ModelImpl> 
    model_base::test_output inline 
    test_model(ModelImpl model, const model_base::testing_input& data, 
            int classes, const char* modelName)
    {
        if (data.empty()) {
            LOG_ERROR<<"No data given for model "<<modelName<<" - failed to test";
            return model_base::test_output{};
        } else {
            LOG_INFO<<"running test for "<<modelName<<" with test data of "<<data.size()
                    <<"rows and "<<data[0].size()<<" columns and "<<classes<<" number of classes";
            Model m{};
            return m.test(model, data, classes);
        }
    }

    struct null_model
    {
        using model_type = undefined_model;

        model_type train(const model_base::training_data&, const model_base::labels_type&, null_args) const
        {
            return model_type{};
        }

        bool predict(model_type ,
                const model_base::testing_input&, model_base::test_output&) const
        {
            return false;
        }
    };
}   // end of local namespace

///////////////////////////////////////////////////////////////////////////////
SVM::model_type SVM::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model::model_type{};
    } else {
        LOG_INFO<<"running training for SVM with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        return train_model(model{}, data, labels, a);
    }
}

SVM::test_output SVM::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "SVM");
}

bool SVM::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train";
        return false;
    } else {
        LOG_INFO<<"running test for SVM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        return do_predication(model{}, m, data, res);
    }
}

bool SVM::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model SVM to '"<<at<<"'";
        mlmodels::save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

model_base::prediction_value SVM::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for SVM model";
    model mdl;
    return mdl.predict(m, row); 
}

std::ostream& operator << (std::ostream& os, const SVM& )
{
    return os<<"SVM classifier";
}

EM::model_type EM::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid data for EM training - empty";
        return EM::model_type{};
    } else {
        LOG_INFO<<"running test for EM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        return train_model(model{}, data, labels, a);
    }
}

EM::test_output EM::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "EM");
}

bool EM::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid data for EM testing - empty";
        return false;
    } else {
        LOG_INFO<<"running test for EM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        return do_predication(model{}, m, data, res);
    }
}

bool EM::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model EM to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

model_base::prediction_value EM::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for EM model";
    model mdl;
    return mdl.predict(m, row); 
}

std::ostream& operator << (std::ostream& os, const EM& )
{
    return os<<"EM";
}

normal_bayes::model_type normal_bayes::train(const training_data& data,
        const labels_type& labels, args) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model_type{};
    } else {
        LOG_INFO<<"running training for normal bayes  with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        model trainer;
        return trainer.train(data, labels);
    }
}

normal_bayes::test_output normal_bayes::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "normal_bayes");
}

bool normal_bayes::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for normal bayes  train";
        return false;
    } else {
        LOG_INFO<<"running test for SVM with test data of "<<data.size()
            <<"rows and "<<data[0].size()<<" columns";
        //dump_model(model);
        return do_predication(model{}, m, data, res);
    }
}

model_base::prediction_value normal_bayes::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for normal_bayes model";
    model mdl;
    return mdl.predict(m, row); 
}

bool normal_bayes::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model normal bayes to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}


std::ostream& operator << (std::ostream& os, const normal_bayes&)
{
    return os<<"normal_bayes";
}

k_nearest::model_type k_nearest::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty() || labels.empty()) {
        LOG_ERROR<<"invalid data given to k_nearest train: data "<<data.size()<<", lavbels: "<<labels.size();
        return model_type{};
    }
    return train_model(model{}, data, labels, a);
}

model_base::prediction_value k_nearest::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for k_nearest model";
    model mdl;
    return mdl.predict(m, row); 
}

k_nearest::test_output k_nearest::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "k_nearest");
}

bool k_nearest::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid data for k_nearest";
        return false;
    }
    return do_predication(model{}, m, data, res);
}

bool k_nearest::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model k -nearset to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

std::ostream& operator << (std::ostream& os, const k_nearest& )
{
    return os<<"k-nearset";
}

random_tree::model_type random_tree::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty() || labels.empty()) {
        LOG_ERROR<<"invalid input for random_tree train model samles: "<<data.size()<<", labels: "<<labels.size();
        return model_type{};
    }
    return train_model(model{}, data, labels, a);
}

model_base::prediction_value random_tree::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for random_tree model";
    model mdl;
    return mdl.predict(m, row); 
}

random_tree::test_output random_tree::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "random_tree");
}

bool random_tree::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"error data given to random tree test model!!";
        return false;
    }

    return do_predication(model{}, m, data, res);
}

bool random_tree::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model random trees to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

std::ostream& operator << (std::ostream& os, const random_tree& )
{
    return os<<"random-tree";
}

logistic_regration::model_type logistic_regration::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model_type{};
    } else {
        LOG_INFO<<"running training for logistic regression classifier with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        return train_model(model{}, data, labels, a);
    }
}

model_base::prediction_value logistic_regration::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for logistic_regration model";
    model mdl;
    return mdl.predict(m, row); 
}

logistic_regration::test_output logistic_regration::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "logistic_regration");
}

bool logistic_regration::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid input given to test model logistic regression classifier";
        return false;
    }
    return do_predication(model{}, m, data, res);
}

bool logistic_regration::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model logistic_regration to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

std::ostream& operator << (std::ostream& os, const logistic_regration& )
{
    return os<<"logistic-regration classifier";
}

boosting_model::model_type boosting_model::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    return train_model(model{}, data, labels, a);
}

model_base::prediction_value boosting_model::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for boosting_model model";
    model mdl;
    return mdl.predict(m, row); 
}

boosting_model::test_output boosting_model::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "boosting_model");
}

bool boosting_model::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    return do_predication(model{}, m, data, res);
}

bool boosting_model::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model boosting to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        LOG_ERROR<<"model is invalid - cannot save!!";
        return false;
    }
}

std::ostream& operator << (std::ostream& os, const boosting_model& )
{
    return os<<"boosting";
}

decision_tree::model_type decision_tree::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty() || labels.empty()) {
        LOG_ERROR<<"invalid input for decision_tree train model samles: "<<data.size()<<", labels: "<<labels.size();
        return model_type{};
    }
    return train_model(model{}, data, labels, a);
}

model_base::prediction_value decision_tree::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for decision_tree model";
    model mdl;
    return mdl.predict(m, row); 
}

bool decision_tree::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid input given to test model logistic regression";
        return false;
    }
    return do_predication(model{}, m, data, res);
}

decision_tree::test_output decision_tree::test(model_type m, 
            const testing_input& data, int classes) const
{
    return test_model<model>(m, data, classes, "decision_tree");
}

bool decision_tree::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model decision_tree to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

std::ostream& operator << (std::ostream& os, const decision_tree& )
{
    return os<<"decision-tree";
}

}   // end of namespace classifier

