#include "apps/run_experiments/process_input.h"
#include "apps/run_experiments/exceptions.h"
#include "logging.h"
#include "libs/json_parser/json_istream.h"
#include "apps/run_experiments/training_input_task.h"
//#include "apps/run_experiments/running_mode.h"
#include "apps/run_experiments/classifersmodels_task.h"
#include "apps/run_experiments/verifiers_task.h"
#include "apps/run_experiments/verifiers.h"
#include "apps/run_experiments/classifers_models.h"
#include "apps/run_experiments/actions_pipeline.h"
#include "apps/run_experiments/classifer_model_interface.h"
#include "libs/classfication_algos/EM.h"
#include <boost/assign.hpp>
#include <map>

namespace
{

using action_parser = pipeline::config::trainers::value_type(*)(json::istream&);
pipeline::config process(json::istream ji);
pipeline::config::path_type get_souce_location(json::istream& input);
bool get_label_id(json::istream& input,
        pipeline::config::label_id& label, unsigned int& iterations);
pipeline::config::experiments get_experiments(json::istream& input);
pipeline::config::experiments labels2experiments(
        const std::vector<std::string>& labels);
pipeline::config::source_policy get_input_type(json::istream& input,
        const pipeline::config::label_id& label);
pipeline::config::trainers get_model_generators(json::istream& input);
pipeline::config::trainers process_actions(json::istream& input);
pipeline::config::trainers::value_type
    process_action_entry(json::istream& ji);
pipeline::config::experiment_call
    process_experiment_entry(const std::string& name);

pipeline::config::source_policy name2input_t(json::istream& input, const std::string& name,
        const pipeline::config::label_id& label);
pipeline::config::source_policy kfold_action(json::istream& input,
        const pipeline::config::label_id& label);
pipeline::config::source_policy percent_action(json::istream& input,
        const pipeline::config::label_id& label);
pipeline::config::trainers::value_type
    generate_svm_action(json::istream& input);
pipeline::config::trainers::value_type
    generate_em_action(json::istream& input);

pipeline::config::trainers::value_type
    generate_normal_bayes_action(json::istream& input);

classifier::expectation_maximization::covariance_matrices_t name2shape(const std::string& sn);

///////////////////////////////////////////////////////////////////////////////
pipeline::config process(json::istream ji)
{
    auto source_location = get_souce_location(ji);
    pipeline::config::label_id id;
    unsigned int iters = 0;
    if (get_label_id(ji, id, iters)) {
        LOG_DEBUG<<"successfully read label id '"<<id<<"' and iterations: "<<iters;
    } else {
        LOG_ERROR<<"cannot process input message, pasrsing failed";
        throw invalid_conf{"invalid configuration failed to process label id or/and iterations count"};
    }
    auto evals = get_experiments(ji);
    auto sr = get_input_type(ji, id);
    auto t = get_model_generators(ji);
    return pipeline::config{source_location, sr, 
        t, evals,
        //std::begin(t), std::end(t),
        //std::begin(evals), std::end(evals),
        iters};
}
pipeline::config::path_type get_souce_location(json::istream& input)
{
    using namespace json::literals;

    if (input) {
        auto p = input ^ json::_child(input, "datasource"_n);
        if (p) {
            std::string path;
            p ^ "path"_n ^ path;
            if (p) {
                return pipeline::config::path_type{path};
            }
        }
        LOG_ERROR<<"failed to read source path from JSON!";
        throw json_error{"failed to read path location from JSON"};
    } else {
        throw json_error{"cannot parse source location - no valid json input"};
    }
}

bool get_label_id(json::istream& input,
        pipeline::config::label_id& label, unsigned int& iterations)
{
    using namespace json::literals;
    if (input) {
        auto c = input ^ json::_child(input, "model"_n);
        if (c) {
            c ^"class"_n ^ label ^ "repeations"_n ^ iterations;
            if (c && !label.empty()) {
                return true;
            } else {
                LOG_ERROR<<"failed to parse model label ID and/or number of repeations";
                throw json_error{"missing 'class' or/and 'repeations' or they have invalid type"};
                return false;
            }
        } else {
            LOG_ERROR<<"no model label found";
            throw json_error{"missing 'model' tag in JSON configuration"};
        } 
    }
    throw json_error{"failed to parse JSON"};
    return false;
}

pipeline::config::experiments get_experiments(json::istream& input)
{
    using namespace json::literals;

    if (input) {
        auto v = input ^ json::_child(input, "evaluation"_n);
        if (v) {
            std::vector<std::string> labels;
            v ^ labels;
            if (!labels.empty()) {
                return labels2experiments(labels);
            } else {
                LOG_ERROR<<"failed to read experiments for models";
                throw json_error{"failed to read list of evaluations from configuration"};
            }
        } else {
            LOG_ERROR<<"no 'evaluation' label in configuration";
            throw json_error{"failed to read evaluations list, missing from configuration"};
        }
    }
    LOG_ERROR<<"cannot process json as we are in invalid state in the json stream";
    throw json_error{"cannot process expriment list invalid input"};
}

pipeline::config::experiments labels2experiments(
        const std::vector<std::string>& labels)
{
    pipeline::config::experiments exprs;
    std::for_each(std::begin(labels), std::end(labels), [&exprs](auto& name) {
                        exprs.push_back(process_experiment_entry(name));
                    } 
    );
    if (exprs.empty()) {
        LOG_ERROR<<"no valid verifiers were found in configuration!!";
        throw invalid_conf{"no valid verifiers in configuration"};
    }
    return exprs;
}

pipeline::config::experiment_call
    process_experiment_entry(const std::string& name)
{
    static const std::map<std::string, pipeline::config::experiment_call> name2actions = 
        boost::assign::map_list_of("auc-roc", pipeline::config::experiment_call{AUC_ROC{}})
                                  ("logisic-los", pipeline::config::experiment_call{logistic_loss{}})
                                  ("ks-test", pipeline::config::experiment_call{kalmogorov_smirnov_test{}});
    auto i = name2actions.find(name);
    if (i == std::end(name2actions)) {
        LOG_ERROR<<"invalid verifier name '"<<name<<"' in configuration";
        throw invalid_conf{"uknown entry for experimention " + name};
    } else {
        LOG_DEBUG<<"creating new algorithm: "<<i->first;
        return i->second;
    }
}

pipeline::config::source_policy get_input_type(json::istream& input,
        const pipeline::config::label_id& label)
{
    using namespace json::literals;

    if (input) {
        auto v = input ^ json::_child(input, "validation"_n);
        if (v) {
            std::string name;
            v ^ "action"_n ^ name;
            return name2input_t(v, name, label);
        } else {
            LOG_ERROR<<"missing 'validation' tag in configuration";
            throw json_error{"missing 'validation' in configuration"};
        }
    }

    LOG_ERROR<<"invalid json - cannot process";
    throw json_error{"cannot process validation type,  invalid input"};
}

pipeline::config::source_policy name2input_t(json::istream& input, 
        const std::string& name, const pipeline::config::label_id& label)
{
    if (input) {
        if (name == "k-fold") {
            return kfold_action(input, label);
        } else if (name == "precent") {
            return percent_action(input, label);
        } else {
            LOG_ERROR<<"uknown action type '"<<name<<"'";
            throw json_error{"unsupported validation name " + name};
        }
    }
    LOG_ERROR<<"cannot read validation action type - invalid input";
    throw json_error{"cannot read validation action type - invalid input"};
}

pipeline::config::source_policy kfold_action(json::istream& input,
        const pipeline::config::label_id& label)
{
    using namespace json::literals;

    auto a = input ^ json::_child(input, "attr"_n);
    if (a) {
        unsigned int s = 0;
        a ^ "arg"_n ^ s;
        return pipeline::config::source_policy{k_fold{s, label}};
    }
    LOG_ERROR<<"missing attributes for k fold input type";
    throw json_error{"no attribute in k fold"};
}

pipeline::config::source_policy percent_action(json::istream& input,
        const pipeline::config::label_id& label)
{
    using namespace json::literals;

    auto a = input ^ json::_child(input, "attr"_n);
    if (a) {
        unsigned int s = 0;
        a ^ "arg"_n ^ s;
        return pipeline::config::source_policy{percent_split{s, label}};
    }
    LOG_ERROR<<"missing attributes for precent input type";
    throw json_error{"no attribute in precent"};
}

pipeline::config::trainers::value_type
    process_action_entry(json::istream& ji)
{
    using namespace json::literals;
    static const std::map<std::string, action_parser> name2action = 
                    boost::assign::map_list_of("model.svm", generate_svm_action)
                    ("model.em", generate_em_action)
                    ("model.normal-bayes", generate_normal_bayes_action);
    std::string n;
    ji ^ "action"_n ^ n;
    auto i = name2action.find(n);
    if (i == std::end(name2action)) {
        LOG_ERROR<<"unknown model type '"<<n<<"'";
        throw invalid_conf{"invalid entry " + n};
    } else {
        LOG_DEBUG<<"creating new algo: "<<i->first;
        return i->second(ji);
    }
}

pipeline::config::trainers::value_type
generate_normal_bayes_action(json::istream& input)
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        return pipeline::config::trainers::value_type{normal_bayes_train{}};
    }
    LOG_ERROR<<"invalid normal bayes model definition at configuration";
    throw invalid_conf{"failed to generate model normal bayes from configuration"};
}

pipeline::config::trainers::value_type
    generate_svm_action(json::istream& input)
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        return pipeline::config::trainers::value_type{SVM_train{}};
    }
    LOG_ERROR<<"invalid SVM model definition at configuration";
    throw invalid_conf{"failed to generate model SVM from configuration"};
}

pipeline::config::trainers::value_type
    generate_em_action(json::istream& input)
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        int cc = -1, mic = -1;
        float tc = 0.f;
        std::string s;
        at ^ "clusters"_n ^ cc ^ "cluster-min"_n ^ mic ^ "temination-count"_n ^ tc ^ "shape"_n ^ s; 
        if (at) {
            return pipeline::config::trainers::value_type{EM_train{cc, mic, 
                                                            tc, name2shape(s)}
                                            };
        } else {
            LOG_ERROR<<"invalid EM model args";
            throw json_error{"failed to parse EM args"};
        }
    }
    LOG_ERROR<<"invalid EM model definition at configuration";
    throw invalid_conf{"failed to generate model EM from configuration"};
}

classifier::expectation_maximization::covariance_matrices_t name2shape(const std::string& sn)
{
    static const std::map<std::string, classifier::expectation_maximization::covariance_matrices_t> s2t = 
            boost::assign::map_list_of("spherical", classifier::expectation_maximization::SPHERICAL)
                                      ("diagonal", classifier::expectation_maximization::DIAGONAL)
                                      ("generic", classifier::expectation_maximization::GENERIC);
    auto i = s2t.find(sn);
    if (i == std::end(s2t)) {
        throw json_error{"invalid EM matrix shape " + sn};
    } else {
        return i->second;
    }
}

pipeline::config::trainers process_actions(json::istream& input)
{
    pipeline::config::trainers actions;
    std::for_each(input.entries().begin(), input.entries().end(), [&actions](auto& entry) {
                std::string n;
                json::istream tmp(entry.second);
                actions.push_back(process_action_entry(tmp));
            }
    );
    if (actions.empty()) {
        LOG_ERROR<<"no models were defined in the configuration";
        throw invalid_conf{"no valid models were found in the configuration"};
    }
    return actions;
}
pipeline::config::trainers get_model_generators(json::istream& input)
{
    using namespace json::literals;

    if (input) {
        auto a = input ^ json::_child(input, "actions"_n);
        if (a) {
            //return pipeline::config::trainers{};
            return process_actions(a);
        } else {
            LOG_ERROR<<"missing attribute actions in configuration";
            throw json_error{"missing actions in configuraiton"};
        }
    }
    LOG_ERROR<<"cannot read actions - invalid JSON";
    throw json_error{"invalid json - cannot process actions"};
}
}       // end of local namespace

///////////////////////////////////////////////////////////////////////////////
pipeline::config process_input(const std::string& from)
{
    static constexpr auto max_path = 256u;

    if (from.empty()) {
        throw invalid_conf{"empty configuration input"};
    } 

    json::istream_root root;
    if (from.size() < max_path) {
        boost::filesystem::path p{from};
        if (boost::filesystem::exists(p)) {
            return process(root ^ p);
        } else {
            throw invalid_conf{"no such file: " + from};
        }
    } else {
        return process(root ^ from);
    }
}

bool good(const pipeline::config& c) 
{
    return pipeline::good(c);
}
