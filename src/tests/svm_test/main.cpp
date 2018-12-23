#include "svm_test_input.hpp"
//#include "skin_data.h"
#include "small_skin_data.h"
#include "../../libs/ml_models/src/svm_impl/svm_model.h"
#include "../../libs/ml_models/src/svm_impl/classifiers_generator.h"
#include "../../libs/ml_models/src/svm_impl/regressions_generator.h"
#include "utils/algorithms.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <chrono>
namespace 
{

const mlmodels::class_data labels = { 0, 0, 1, 1, 0, 2, 1, 2, 1, 0 };

const mlmodels::training_data dataf = make_multi_array<mlmodels::value_type, 10, 2>(
        { { 100.f, 10.f }, 
          { 150.f, 10.f }, 
          { 600.f, 200.f }, 
          { 600.f, 10.f }, 
          { 10.f, 100.f }, 
          { 455.f, 10.f }, 
          { 345.f, 255.f }, 
          { 10.f, 501.f }, 
          { 401.f, 255.f }, 
          { 30.f, 150.f } 
        }
);

void run_prediction(const mlmodels::training_data& data,
        const mlmodels::class_data& labels, const std::vector<float>& expected,
        const mlmodels::training_data&,
        double c, double g);

void print_prediction(const mlmodels::svm::predictions_list& predictions)
{
    unsigned int rn = 0;
    std::for_each(std::begin(predictions), std::end(predictions), [&rn](const auto& row) {
            std::cout<<"row "<<rn++<<" [";
            out_range(std::cout, row);
            std::cout<<"]"<<std::endl;
        }
    );
}

void test_model_acc()
{
    run_prediction(small_skin_train_features, 
            small_skin_labels, small_skin_predictions,
            small_skin_test_features, 512, 2);

}

void run_prediction(const mlmodels::training_data& data,
        const mlmodels::class_data& labels, const std::vector<float>& expected,
        const mlmodels::training_data& test_features,
        double C, double gamma)
{
    using namespace mlmodels;
    auto start = std::chrono::system_clock::now();
    auto model = svm::classifier::create_model(svm::classifier::c_rbf_train{
                                                        svm::classifier::c_rbf_train::base_1
                                                        { 
                                                            svm::rbf_args{gamma}, 
                                                            svm::base_train_params{}
                                                        },
                                                        svm::classifier::c_rbf_train::base_2{
                                                            C, 0, 0
                                                        }
                                        },
                                        data, labels
                                    );
    auto te = std::chrono::system_clock::now(); 
    std::chrono::duration<double> el{te - start};
    std::cout<<"it took "<< el.count()
        <<" second for training"<<std::endl;
   if ((model.get() == nullptr)) {
       std::cerr<< "we should get a valid model here\n";
       return;
   }
//   svm::save(model, "/tmp/test_model.txt");
   auto pred = svm::predict(model, test_features);
   auto pe = std::chrono::system_clock::now();
   el = pe - te;
    std::cout<<"it took "<< el.count()
        <<" second for predictions"<<std::endl;
   if (pred.empty()) {
       std::cerr<< "we should be getting a valid list of prediction here\n";
       return;
   }
   if (pred.size() != expected.size()) {
       std::cerr<<"model prediction size "<<expected.size() << " != "<<" our size "<<pred.size()<<std::endl;
       return;
   }
    auto expted_begin = std::begin(expected);
    unsigned int at = 0;
    for (auto our_results : pred) {
        if (our_results != *expted_begin) {
            std::cerr<< "["<<at<<"] our results "<<our_results<<" do not match expected results "<<*expted_begin<<"\n";
        }
        ++expted_begin;
        ++at;
    }
    std::cout<<"finish testing model with "<<pred.size()<<" predictions"<<std::endl;
}

}   // end of namespace

int main()
{
    using namespace mlmodels;
    // do the same for regression model
    if (false) { 
    try {
        //auto model = svm::regression::create_model(svm::regression::epsilon_linear_train{}, dataf, labels);
        auto model = svm::classifier::create_model(svm::classifier::c_linear_train{}, dataf, labels);
        if (!model) {
            std::cerr<<"failed to  create model!!\n";
            return -1;
        }
        // pass a row of valid size for prediction
        const class_data row{11.f, 110.f};
        auto ret = svm::test_detailed(model, dataf);
        if (ret.empty()) {
            std::cerr<<"the args to the model test is  - good, should not return empty\n";
            return -1;
        } else {
            print_prediction(ret);
        }
        auto ret2 = svm::predict(model, row);
        if (ret2 == std::numeric_limits<value_type>::max()) {
            std::cerr<< "the args to predict are good, we pass good number of features\n";
        } else {
            std::cout<<"prediction "<<ret2<<std::endl;
        }
    } catch (...) {
        std::cerr<<"we should not get any exception here\n";
        return -2;
    }
    // in this test we would make sure that if we are passing a valid arguments we are getting
    // valid results - by valid it means that they are not out of bound, but we are not checking 
    // if this is a correct prediction mathematically
    try {
        auto model = svm::classifier::create_model(svm::classifier::c_linear_train{}, train_data, train_labels);
        if (!model) {
            std::cerr<<"failed to  create model!!\n";
            return -1;
        }
        // pass a row of valid size for prediction
        const class_data row{0.0416667, -1,-0.333333, -0.283019, -0.260274, 1, 1, 0.343511, 1, -1, -1, -0.333333, -1};
        auto ret = svm::test_detailed(model, test_data);
        if (ret.empty()) {
            std::cerr<<"the args to the model test is  - good, should not return empty\n";
            return -1;
        } else {
            print_prediction(ret);
        }
        auto ret2 = svm::predict(model, row);
        if (ret2 == std::numeric_limits<value_type>::max()) {
            std::cerr<< "the args to predict are good, we pass good number of features\n";
        } else {
            std::cout<<"prediction "<<ret2<<std::endl;
        }
        ret = svm::test_detailed(model, test_data);
        if (ret.empty()) {
            std::cerr<<"the args to the model test is  - good, should not return empty\n";
            return -1;
        } else {
            print_prediction(ret);
        }
        ret2 = svm::predict(model, row);
        if (ret2 == std::numeric_limits<value_type>::max()) {
            std::cerr<< "the args to predict are good, we pass good number of features\n";
        } else {
            std::cout<<"prediction "<<ret2<<std::endl;
        }
//        return 0;
        auto p = svm::test(model, test_data);
        if (not p.empty()) {
            std::cout<<"predictions are: {";
            out_range(std::cout, p);
            std::cout<<"}"<<std::endl;
        } else {
            std::cout<<"failed to run test!!"<<std::endl;
            return -2;
        }
    } catch (...) {
        std::cerr<<"we should not get any exception here\n";
        return -2;
    }
    }
    test_model_acc();
    
}
