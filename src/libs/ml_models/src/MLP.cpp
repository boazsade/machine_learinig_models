#include "libs/ml_models/MLP.h"
#include "shared_operations.hpp"
#include "utils/missing_values.hpp"
#include <opencv/ml.h>
#include <opencv/cxcore.h>

namespace mlmodels
{

namespace 
{
cv::ml::ANN_MLP::TrainingMethods 
    convert(multi_layer_perceptrons::args::train_method_t from)
{
    switch (from) {
        case multi_layer_perceptrons::args::BACKPROP:
            return cv::ml::ANN_MLP::BACKPROP;
        case multi_layer_perceptrons::args::RPROP:
            return cv::ml::ANN_MLP::RPROP;
    }
}

cv::ml::ANN_MLP::ActivationFunctions 
    convert(multi_layer_perceptrons::args::activation_function_t )
{
    return cv::ml::ANN_MLP::SIGMOID_SYM;    // this is the only supported option
}
}   // end of local namespace 

multi_layer_perceptrons::args::args(int iters, double ep, 
                                    activation_function af, train_method tm,
                                    const layers_t& l) :
            iterations(iters), epsilon(ep), function(af), train(tm), 
            layers(l.size() + 2)    // we allocate to two more since we are adding the first and last layer
{
    std::copy(std::begin(l), std::end(l), std::begin(layers)+1);
    layers[layers.size()-1] = 1;    // output layer only one
}

multi_layer_perceptrons::args::train_method_t 
    multi_layer_perceptrons::args::string2method(const std::string& from)
{
    if (from == "rprop") {
        return RPROP;
    } else if (from == "backprop") {
        return BACKPROP;
    } else {
        throw std::runtime_error{"invalid train method type " + from};
    }
}


struct MLP_impl : detail::general_model<cv::ml::ANN_MLP>
{
    using base_t = detail::general_model<cv::ml::ANN_MLP>;
    using base_t::base_t;

    MLP_impl(const multi_layer_perceptrons::args& arg, int columns) : base_t{}
    {
        auto af = convert(arg.function.type);
        if (af != cv::ml::ANN_MLP::SIGMOID_SYM) {
            throw std::runtime_error{"invalid type for activation function type"};
        }
        set_layers(arg.layers, columns);
        handle->setActivationFunction(af/*, arg.function.alpha, arg.function.beta*/);
        handle->setTermCriteria(cv::TermCriteria(cv::TermCriteria::Type::COUNT + cv::TermCriteria::Type::EPS,
                        arg.iterations, arg.epsilon)
                );
        handle->setTrainMethod(convert(arg.train.type)/*, arg.train.arg_1, arg.train.arg_2*/);
    }

    void set_layers(multi_layer_perceptrons::layers_t l, 
            int columns)
    {
        if (l.size() < 2u) {    // we must have at least 2 layers
           l = multi_layer_perceptrons::args::default_layers(); 
        }
        l[0] = columns;
        handle->setLayerSizes(l);
    }
};

value_type multi_layer_perceptrons::predict(model_type model,
                const class_data& samples) const
{
    if (model) {
        return model->predict(samples);
    }
    return missing_value<value_type>();
}

bool multi_layer_perceptrons::predict(model_type model, 
        const testing_data& data,
        testing_result& predications) const
{
    if (model) {
        return model->predict(data, predications);
    } else {
        return false;
    }
}

class_data multi_layer_perceptrons::test(model_type model, 
        const testing_data& data, int) const
{
    if (model) {
        return model->test(data, -1);
    } else {
        return class_data{};
    }
}

void save_model(multi_layer_perceptrons::model_type model,
        const char* path)
{
    if (model) {
        model->save(path);
    }
}

multi_layer_perceptrons::model_type 
multi_layer_perceptrons::train(const training_data& data,
            const class_data& classes, const args& a) const
{

    if (!(data.empty() || classes.empty())) {
        model_type m(std::make_shared<MLP_impl>(a, columns(data)));
        if (m->reg_train(data, classes)) {
            return m;
        }
    }
    return model_type{};
}

multi_layer_perceptrons::model_type
    multi_layer_perceptrons::load(const char* from)
{
    return model_type{std::make_shared<MLP_impl>(MLP_impl::load(from))};
}

const multi_layer_perceptrons::layers_t 
    multi_layer_perceptrons::args::default_layers()
{
    return layers_t{
        layers_t::value_type{}, // this is a placeholder that we would later fill with value based on the input
        first_layer_size, 
        first_layer_size/default_layer_factor, 
        1 // the last layer size is 1 since the output is signle target
    };
}

}   // end of namespace mlmodels

