#pragma once
#include "general_types.h"
#include "model_types.h"
#include <memory>
#include <string>
#include <vector>

namespace mlmodels
{

struct MLP_impl;

struct multi_layer_perceptrons :
    regression_model
{

    using model_type = std::shared_ptr<MLP_impl>;
    using layers_t = std::vector<int>;

    static const unsigned int default_layers = 2;
    static const layers_t::value_type first_layer_size = 8;
    static const layers_t::value_type default_layer_factor = 2;

    struct args
    {
        enum activation_function_t
        {
            IDENTITY,
            SIGMOID_SYM,    // only this one is supported
            GAUSSIAN
        };

        enum train_flag_t
        {
            UPDATE_WEIGHTS,
            NO_INPUT_SCALE,
            NO_OUTPUT_SCALE
        };

        enum train_method_t
        {
            BACKPROP,
            RPROP       // the default 
        };

        struct activation_function
        {
            activation_function_t type = SIGMOID_SYM;
            double alpha = 0;
            double beta = 0;

            constexpr activation_function() = default;
            constexpr activation_function(double a, double b) :
                    type(SIGMOID_SYM), alpha(a), beta(b)
            {
            }
        };

        struct train_method
        {
            train_method_t type = RPROP;
            double arg_1 = 0.1;
            double arg_2 = 0.0001;

            constexpr train_method() = default;
            constexpr train_method(train_method_t t, double x, double y) :
                    type(t), arg_1(x), arg_2(y)
            {
            }
        };

        int iterations = 1000;
        double epsilon = 0.01;
        activation_function function = activation_function{};
        train_method train = train_method{};
        layers_t layers = default_layers();

        args() = default;

        args(int iters, double ep, activation_function af, 
            train_method tm, const layers_t& l);

        static train_method_t string2method(const std::string& from);

        static const layers_t default_layers();
    };

    constexpr multi_layer_perceptrons() = default;

    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes, const args& a) const;

    bool predict(model_type model, const testing_data& data,
            testing_result& predications) const;

    class_data test(model_type model, const testing_data& data, int) const;

    value_type predict(model_type model,
                const class_data& samples) const;


};

void save_model(multi_layer_perceptrons::model_type model,
        const char* to);

}   // end of namespace mlmodels

