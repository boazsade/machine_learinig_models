#include "libs/regression_models_actions/regression_models.h"
#include "logging.h"
#include <iostream>


namespace regression
{

MLP_model::model_type MLP_model::train(const training_data& input,
            const labels_type& lablels, const args& a) const
{
    if (input.empty()) {
        LOG_ERROR<<"MLP train model called with empty data";
        return MLP_model::model_type{};
    } else {
        LOG_INFO<<"running MLP model with data size "<<input.size()<<" rows by "
            <<input[0].size()<<" columns";
        model m;
        return m.train(input, lablels, a);
    }
}

bool MLP_model::predict(model_type m,
            const testing_input& input,
            test_output& result) const
{
    if (input.empty()) {
        LOG_ERROR<<"invalid input given to test model MLP regression";
        return false;
    }
    model ml;
    return ml.predict(m, input, result);
}

MLP_model::test_output MLP_model::test(model_type m,
            const testing_input& input,
            int) const
{
    if (m && !input.empty()) {
        LOG_INFO<<"running MLP test model";
        model ml;
        return ml.test(m, input, -1);
    } else {
        LOG_ERROR<<"cannot run MLP model - "<<(m ? "empty input" : "invalid model handler");
        return test_output{};
    }
}

bool MLP_model::save(model_type m, const std::string& at) const
{
    if (m) {
        LOG_INFO<<"saving model MLP to '"<<at<<"'";
        save_model(m, at.c_str());
        return true;
    } else {
        return false;
    }
}

model_base::prediction_value 
    MLP_model::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for MLP model";
    model ml;
    return ml.predict(m, row);
}

std::ostream& operator << (std::ostream& os, const MLP_model& )
{
    return os<<"multi layer perceptrons regression";
}


logistic_regration::model_type logistic_regration::train(const training_data& data,
        const labels_type& labels, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model_type{};
    } else {
        LOG_INFO<<"running training for logistic regression  with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        model m;
        return m.train(data, labels, a);
    }
}

logistic_regration::test_output logistic_regration::test(model_type m,
            const testing_input& input,
            int) const
{
    if (m && !input.empty()) {
        LOG_INFO<<"running logistic_regration test model";
        model ml;
        return ml.test(m, input, -1);
    } else {
        LOG_ERROR<<"cannot run logistic_regration model - "<<(m ? "empty input" : "invalid model handler");
        return test_output{};
    }
}

bool logistic_regration::predict(model_type m, const testing_input& data,
        test_output& res) const
{
    if (data.empty()) {
        LOG_ERROR<<"invalid input given to test model logistic regression regression";
        return false;
    }
    model ml;
    return ml.predict(m, data, res);
}

model_base::prediction_value 
    logistic_regration::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for logistic_regration model";
    model ml;
    return ml.predict(m, row);
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
    return os<<"logistic-regration classifier regression";
}


SVM::model_type SVM::train(const training_data& data,
                        const labels_type& l, const args& a) const
{
    if (data.empty()) {
        LOG_ERROR<<"got invalid data for SVM train!!";
        return model::model_type{};
    } else {
        LOG_INFO<<"running training for SVM with "<<data.size()
            <<" rows and "<<data[0].size()<<" columns";
        model m;
        return m.train(data, l, a);
    }
}

model_base::prediction_value 
    SVM::predict(model_type m, const row_type& row) const
{
    LOG_DEBUG<<"running prediction for SVM model";
    model ml;
    return ml.predict(m, row);
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
        model ml;
        return ml.predict(m, data, res);
    }
}

SVM::test_output SVM::test(model_type m,
            const testing_input& input,
            int) const
{
    if (m && !input.empty()) {
        LOG_INFO<<"running SVM test model";
        model ml;
        return ml.test(m, input, -1);
    } else {
        LOG_ERROR<<"cannot run SVM model - "<<(m ? "empty input" : "invalid model handler");
        return test_output{};
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

std::ostream& operator << (std::ostream& os, const SVM& )
{
    return os<<"SVM regression";
}

}   // end of namespace regression
