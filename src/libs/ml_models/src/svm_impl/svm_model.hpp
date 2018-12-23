#pragma once
#include "svm_model.h"
#include "params_io.h"
#include <boost/filesystem.hpp>
#include <svm.h>
#include <algorithm>
#include <vector>
//#include <iostream>

namespace mlmodels
{
namespace svm
{

namespace
{

using node_data = std::vector<svm_node>;

const auto UNKOWN_FEATURES_COUNT = 9999999u;

template<typename It>
value_type best_prob(It from, It to)
{
    std::sort(from, to, [](auto left, auto right) {
                    return left > right;
                }
    );
    return *from;
}

template<typename T>
T* allocate(std::size_t nodes)
{
    return static_cast<T*>(malloc(sizeof(T) *nodes));
}

template<typename T>
void deallocate(T* node)
{
    free(node);
}

template<typename It>
void fill_node(svm_node* row_node, It row_begin, It row_end)
{
    unsigned int index = 1;
    std::for_each(row_begin, row_end, [&row_node, &index](auto val) {
            row_node->index = index++;
            row_node->value = val;
            ++row_node;
        }
    );
    row_node->index = -1;
}

template<typename It>
svm_node* allocate_node(It row_begin, It row_end, std::size_t cols)
{
    auto row_node = allocate<svm_node>(cols+1);
    fill_node(row_node, row_begin, row_end);
    return row_node;
}


template<typename It>
value_type run_prediction(It row_begin, It row_end, std::size_t features,
        double* prob_est, svm_node* pred_arg, const svm_model* model)
{
    fill_node(pred_arg, row_begin, row_end);
    svm_predict_probability(model, pred_arg, prob_est);
    return best_prob(prob_est, prob_est + features);
}

bool do_validate(const model_type& model, std::size_t columns); 

inline bool validate(const model_type& model, const class_data& cd) {
    return do_validate(model, cd.size());
}

inline bool validate(const model_type& model, const training_data& td) {
    return do_validate(model, columns(td));
}

using prediction_function_t = value_type(*)(const svm_model*, 
                                double* , const node_data&, unsigned int);
value_type predict_no_probs(const svm_model*, 
                    double* , const node_data&, unsigned int);
value_type predict_by_probs(const svm_model*,
                    double* , const node_data&, unsigned int);

prediction_function_t assign_prob_func(const svm_parameter& params)
{
    switch (params.svm_type) {
        case ::NU_SVR:
        case ::EPSILON_SVR:
        case ::ONE_CLASS:
            return predict_no_probs;
        default:
            return params.probability ? predict_by_probs : predict_no_probs;
    }
}

value_type predict_no_probs(const svm_model* the_model, 
                double* , const node_data& pred_arg, unsigned int)
{
    return static_cast<value_type>(svm_predict(the_model, pred_arg.data()));
}

value_type predict_by_probs(const svm_model* the_model, 
                double* prob_est, const node_data& pred_arg, unsigned int classes)
{
    svm_predict_probability(the_model, pred_arg.data(), prob_est);
    return best_prob(prob_est, prob_est + classes);
}

}   // end of local namespace

void release_model(svm_model* m)
{
    svm_free_and_destroy_model(&m);
}

struct model_args
{
    svm_problem data;

    model_args() 
    {
        data.l = -1;
        data.y = nullptr;
        data.x = nullptr;
    }

    model_args(const training_data& from, const class_data& labels) : model_args{}
    {
        using range = training_data::index_range;
        if (rows(from) != labels.size()) {
            throw std::runtime_error{"invalid arguments for svm model training - number of rows for data " + 
               std::to_string(rows(from)) + " != " + std::to_string(labels.size())}; 
        }

        data.l = rows(from);    // number of rows is the length
        data.y = allocate<double>(rows(from));
        data.x = allocate<svm_node*>(rows(from));
        auto cols = columns(from);
        for (auto r = 0; r < data.l; ++r) {
            auto row = from[boost::indices[r][range()]];
            data.x[r] = allocate_node(row.begin(), row.end(), cols);
            data.y[r] = labels[r];
        }
    }

    ~model_args()
    {
        if (data.l > 0 && data.x && data.y) {
            for (auto i = 0; i < data.l; i++) {
                   deallocate<svm_node>(data.x[i]);
            }
            deallocate<svm_node*>(data.x);
            deallocate<double>(data.y);
        }
    }


};

const svm_problem* recast(const model_args& from) {
    return &from.data;
}

svm_model* run_train(const model_args& arg, const ::svm_parameter& params)
{
 //   std::cout<<"start running model train, with args:\n"<<params<<std::endl;
    return ::svm_train(recast(arg), &params);
 //   std::cout<<"finish model training"<<std::endl;
}

struct model_impl
{
    using training_args = model_args;
    using model_type = svm_model*;

    training_args args4train;
    model_type the_model = nullptr;
    unsigned int features_count = 0;

    model_impl()
    {
    }

    model_impl(const svm_parameter& params, 
            const training_data& from, const class_data& labels) : 
                    args4train{from, labels},
                    the_model{nullptr},
                    features_count{0u},
                    predict_op{assign_prob_func(params)}
    {
  //      std::cout<<"run training on "<<columns(from)<<" X "<<rows(from)<< " of data and "<<labels.size()<<" rows of labels"<<std::endl;
        the_model = run_train(args4train, params);
        if (the_model) {
            features_count = columns(from);
        }
    }

    ~model_impl() 
    {
        if (the_model) {
            svm_free_and_destroy_model(&the_model);
        }
    }

    explicit model_impl(const char* from) :
            args4train{},
            the_model{svm_load_model(from)}
    {
        if (the_model) {
            features_count = UNKOWN_FEATURES_COUNT;
        }
    }

    unsigned int features() const
    {
        return features_count;
    }

    bool good() const {
        return features() != 0u;
    }

    bool correct(const testing_data& data) const {
        return correct(columns(data));
    }

    bool correct(const class_data& data) const {
        return correct(data.size());
    }

    bool correct(std::size_t size) const {
        return the_model && size != 0 && (features() == UNKOWN_FEATURES_COUNT || features() == size);
    }

    class_data test(const testing_data& data) const
    {
        using range = training_data::index_range;

        if (correct(data)) {
            auto classes = svm_get_nr_class(the_model); 
            std::vector<double> prob_est(classes, -17.f);
            auto r = rows(data);
            auto cols = columns(data);
            class_data ans(r, 0);
            node_data pred_arg{cols+1}; 
            for (auto i = 0u; i < r; ++i) {
                auto row = data[boost::indices[i][range()]];
                fill_node(pred_arg.data(), row.begin(), row.end());
                //svm_predict_probability(the_model, pred_arg.data(), prob_est.data());
                ans[i] = predict_op(the_model, prob_est.data(), pred_arg, classes); //best_prob(std::begin(prob_est), std::end(prob_est));
            }
            return ans;
        }
        return class_data{};
    }

    value_type predict(const class_data& samples) const
    {
        if (correct(samples)) {
            node_data pred_arg{samples.size()+1};
            fill_node(pred_arg.data(), std::begin(samples), std::end(samples));
            return predict_no_probs(the_model, nullptr, pred_arg, 0u);
        }
        return std::numeric_limits<value_type>::max();
    }

    testing_result predict(const testing_data& data) const
    {
        using range = training_data::index_range;

        if (correct(data)) {
            auto r = rows(data);
            auto cols = columns(data);
            testing_result output(r, testing_result::value_type{});
            node_data pred_arg{cols+1};
            for (auto i = 0u; i < r; ++i) {
                auto row = data[boost::indices[i][range()]];
                fill_node(pred_arg.data(), row.begin(), row.end());
                output[i] = static_cast<value_type>(svm_predict(the_model, pred_arg.data()));
            }
            return output;

        }
        return testing_result{};
    }

    bool save(const char* to) const
    {
        if (the_model) {
            return svm_save_model(to, the_model) > -1;
        } else {
            return false;
        }
    }
    predictions_list detailed_testing(const testing_data& data) const
    {
        using range = training_data::index_range;

        if (correct(data)) {
            auto classes = svm_get_nr_class(the_model); 
            std::vector<double> prob_est(classes, -17.f);
            auto r = rows(data);
            auto cols = columns(data);
            predictions_list ans;

            node_data pred_arg{cols+1};
            for (auto i = 0u; i < r; ++i) {
                auto row = data[boost::indices[i][range()]];
                fill_node(pred_arg.data(), row.begin(), row.end());
                svm_predict_probability(the_model, pred_arg.data(), prob_est.data());
                ans.push_back(prob_est);
            }
            return ans;
        }
        return predictions_list{};
    }

private:
    prediction_function_t predict_op = nullptr;
};

bool save(model_type model, const char* to)
{
    if (!model) {
        return false;
    }
    boost::filesystem::path p{to};
    if (boost::filesystem::exists(p.parent_path())) {
        return model->save(to);
    } else {
        return false;
    }    
}

model_type load(const char* from)
{
    boost::filesystem::path p{from};
    if (boost::filesystem::exists(p)) {
        return std::make_shared<model_impl>(from);
    }
    return model_type{};
    
}

model_type create(const svm_parameter& params, 
                    const training_data& data, const class_data& targets)
{
    if (empty(data) || targets.empty() || rows(data) != targets.size()) {
        return model_type{};
    } else {
        return std::make_shared<model_impl>(params, data, targets);
    }
}

class_data test(model_type model, const testing_data& data)
{
    if (validate(model, data)) {
        return model->test(data);
    }
    return class_data{};
}

value_type predict(model_type model, const class_data& samples)
{
    if (validate(model, samples)) {
        return model->predict(samples);
    }
    return std::numeric_limits<value_type>::max();
}

testing_result predict(model_type model, const testing_data& data)
{
    if (validate(model, data)) {
        return model->predict(data);
    }
    return testing_result{};
}

predictions_list test_detailed(model_type model, const testing_data& data)
{
    if (model) {
        return model->detailed_testing(data);
    }
    return predictions_list{};
}

namespace {

bool do_validate(const model_type& model, std::size_t columns) {
    return model && model->correct(columns);
}


}   // end of local namespace
}   // end of namesapce svm
}   // end of mlmodels

