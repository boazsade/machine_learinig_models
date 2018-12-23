#pragma once
#include "general_types.h"
#include "model_types.h"
#include "svm_types.h"
#include "svm_impl/modelfwd.h"
#include <string>
#include <memory>

namespace mlmodels
{


struct support_vector_machine
{
    using model_type = svm::model_type;
    using row_output = class_data;

protected:
    struct _args
    {

        svm::kernel_type kernel = svm::RBF;
        svm::type_t type = svm::C_SVC;


        _args() = default;
        explicit _args(svm::kernel_type k, svm::type_t t);
    };

    support_vector_machine() = default;

public:
    static svm::kernel_type string2kernel(const std::string& from);
    static svm::type_t string2type(const std::string& from);


    // when loading - you don't need to train
    static model_type load(const char* from);

    bool predict(model_type model, const testing_data& data,
            testing_result& predications) const;


    value_type predict(model_type model,
                const class_data& samples) const;

protected:
    row_output test(model_type model, const testing_data& data,
            int classes) const; 
};

void save_model(support_vector_machine::model_type model, const char* to);

struct classifier_svm : public support_vector_machine,
                        classifier_model
{
    using base_t = support_vector_machine;
    using base_t::base_t;  

    struct args : 
            base_t::_args
    {
        using base_t = support_vector_machine::_args;

        args();
        explicit args(svm::kernel_type k);
    };

    model_type train(const training_data& data,
            const class_data& classes, const args& a) const;

    row_output test(model_type model, const testing_data& data,
            int classes) const;
};

struct regression_svm : public support_vector_machine,
                        regression_model
{
    using base_t = support_vector_machine;
    using base_t::base_t;  

    struct args : 
            base_t::_args
    {
        using base_t = support_vector_machine::_args;
        args();
        explicit args(svm::kernel_type k);
    };

    model_type train(const training_data& data,
            const class_data& classes, const args& a) const;

    row_output test(model_type model, const testing_data& data,
            int) const;
};

}   // end of namemspace classifier


