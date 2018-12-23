#include "interface.h"
#include "classifersmodels_task.h"
#include "classifer_model_interface.h"
#include "running_mode.h"
#include "verifiers_task.h"
#include <boost/range/algorithm/copy.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

int main()
{
    act::oper::models_generators models_builders{
        {classifier::SVM_train{}},
        {classifier::EM_train{}},
        {classifier::random_tree_train{}},
        {classifier::boosting_train{}}
    };
    std::cout<<"model generation:\n";
    boost::copy(models_builders, std::ostream_iterator<act::oper::models_generators::value_type>(std::cout, "\n"));
    model_generator model{classifier::SVM_train{}};
    extract4training train_data{classifier::percent_split{20u, "my-name-is-mada"}};
    act::oper::experiment_call exper{AUC_ROC{}};
    std::cout<<"the model we've created is "<<model<<std::endl;
    std::cout<<"the extractor for the train is "<<train_data<<std::endl;
    std::cout<<"the experiment for the data is "<<exper<<std::endl;
    act::oper::expers_list experiments{
       logistic_loss{}, kalmogorov_smirnov_test{},
        AUC_ROC{}, kalmogorov_smirnov_test{}
    }; 
    boost::copy(experiments, 
            std::ostream_iterator<act::oper::expers_list::value_type>(std::cout, "' "));
    std::cout<<std::endl;
    return 0;
}

