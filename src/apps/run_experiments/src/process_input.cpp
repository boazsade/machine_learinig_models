#include "apps/run_experiments/process_input.h"
#include "apps/run_experiments/verifiers_task.h"
#include "apps/run_experiments/actions_pipeline.h"
#include "libs/json_parser/json_istream.h"
#include "libs/model_data/training_input_task.h"
#include "libs/system/exceptions.h"
#include "libs/classifier_models_actions/classifersmodels_task.h"
#include "libs/regression_models_actions/regressionsmodels_task.h"
#include "libs/scores_evaluation/verifiers.h"
#include "libs/classifier_models_actions/classifers_models.h"
#include "libs/classifier_models_actions/classifer_model_interface.h"
#include "libs/regression_models_actions/regression_model_interface.h"
#include "libs/scores_evaluation/verifier_types.h"
#include "libs/model_datatypes/model_types_io.h"
#include "libs/ml_models/EM.h"
#include "utils/dc_lookup_table.h"
#include "logging.h"
#include <boost/assign.hpp>
#include <map>

namespace lookup
{
    json::istream& operator ^ (json::istream& is, table_entry& te);
}   // end of namespace lookup

namespace
{


using action_parser = pipeline::config::trainers::value_type(*)(json::istream&, bool);

bool name2type(const std::string& n);
pipeline::config process(json::istream ji);
pipeline::config::path_type get_souce_location(json::istream& input);
bool get_label_id(json::istream& input,
        pipeline::config::label_id& label, unsigned int& iterationsi,
        bool& class_type);
pipeline::config::experiments get_experiments(json::istream& input, bool class_type);
pipeline::config::experiments labels2experiments(
        const std::vector<std::string>& labels, actions::model_type mt);
pipeline::config::source_policy get_input_type(json::istream& input,
        const pipeline::config::label_id& label, bool& rand);
pipeline::config::trainers get_model_generators(json::istream& input, bool class_type);
//lookup::table read_lookup_table(json::istream&);
pipeline::config::trainers process_actions(json::istream& inputi, bool class_type);
pipeline::config::trainers::value_type
    process_action_entry(json::istream& ji, bool class_type);
pipeline::config::experiments::value_type
    process_experiment_entry(const std::string& name, actions::model_type mt);

pipeline::config::source_policy name2input_t(json::istream& input, const std::string& name,
        const pipeline::config::label_id& label, bool& rand);
pipeline::config::source_policy kfold_action(json::istream& input,
        const pipeline::config::label_id& label);
pipeline::config::source_policy percent_action(json::istream& input,
        const pipeline::config::label_id& label, bool& rand);
pipeline::config::trainers::value_type
    generate_svm_action(json::istream& input, bool classifier);
pipeline::config::trainers::value_type
    generate_em_action(json::istream& input, bool classifier);

pipeline::config::trainers::value_type
    generate_normal_bayes_action(json::istream& input, bool classifier);

pipeline::config::trainers::value_type
    generate_mlp_action(json::istream& input, bool classifier);

pipeline::config::trainers::value_type
generate_logistic_regression(json::istream& input, bool classifier);

pipeline::config::trainers::value_type
generate_k_nearest_action(json::istream& input, bool classifier);

pipeline::config::trainers::value_type
    generate_boosting_action(json::istream& input, bool classifier);
pipeline::config::trainers::value_type
    generate_decision_trees_action(json::istream& input, bool classifier);
pipeline::config::trainers::value_type
    generate_random_tree_action(json::istream& input, bool classifier);

mlmodels::expectation_maximization::args::covariance_matrices_t 
    name2shape(const std::string& sn);
classifier::logistic_regression_train::model_args::regularization_t 
    convert_reg(const std::string& from);
classifier::logistic_regression_train::model_args::train_method_t 
    convert_method(const std::string& from);

mlmodels::dtree_args 
    read_dtree_args(json::istream& att, const char* act_model);

///////////////////////////////////////////////////////////////////////////////


bool name2type(const std::string& n)
{
    if (n == "classifier") {
        return true;
    } else if (n == "regression") {
        return false;
    } else {
        LOG_ERROR<<"invalid type given "<<n<<" for model";
        throw invalid_conf{"invalid configuration value given for model type " + n};
    }
}

pipeline::config process(json::istream ji)
{
    auto source_location = get_souce_location(ji);
    pipeline::config::label_id id;
    unsigned int iters = 0;
    bool is_class = true;
    if (get_label_id(ji, id, iters, is_class)) {
        LOG_DEBUG<<"successfully read label id '"<<id<<"' and iterations: "<<iters;
    } else {
        LOG_ERROR<<"cannot process input message, pasrsing failed";
        throw invalid_conf{"invalid configuration failed to process label id or/and iterations count"};
    }
    bool randomized = false;
    auto evals = get_experiments(ji, is_class);
    auto sr = get_input_type(ji, id, randomized);
    auto t = get_model_generators(ji, is_class);
    return pipeline::config {
                source_location, sr, 
                t, evals, iters, 
                lookup::map_values_conf{id, is_class ? actions::CLASSIFIER_MODEL : actions::REGRESSION_MODEL},
                randomized
    };
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
        pipeline::config::label_id& label, unsigned int& iterations,
        bool& class_type)
{
    using namespace json::literals;
    if (input) {
        auto c = input ^ json::_child(input, "model"_n);
        if (c) {
            std::string t;
            c ^"class"_n ^ label ^ "repeations"_n ^ iterations ^ "type"_n ^ t;
            if (c && !label.empty()) {
                class_type = name2type(t);
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

pipeline::config::experiments get_experiments(json::istream& input, bool class_type)
{
    using namespace json::literals;

    if (input) {
        auto v = input ^ json::_child(input, "evaluation"_n);
        if (v) {
            std::vector<std::string> labels;
            v ^ labels;
            if (!labels.empty()) {
                return labels2experiments(labels, 
                        class_type ? actions::CLASSIFIER_MODEL : actions::REGRESSION_MODEL);
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
        const std::vector<std::string>& labels, actions::model_type mt)
{
    pipeline::config::experiments exprs;
    std::for_each(std::begin(labels), std::end(labels), [&exprs, mt](auto& name) {
                        exprs.push_back(process_experiment_entry(name, mt));
                    } 
    );
    if (exprs.empty()) {
        LOG_ERROR<<"no valid verifiers were found in configuration!!";
        throw invalid_conf{"no valid verifiers in configuration"};
    }
    return exprs;
}

pipeline::config::experiments::value_type
    process_experiment_entry(const std::string& name, actions::model_type mt)
{
                                  //("logisic-los", log_loss)
    static const std::map<std::string, pipeline::config::experiments::value_type> name2actions = 
        boost::assign::map_list_of("auc-roc", AUC_ROC_t)
                                  ("mse", MSE)
                                  ("cm", confusion_matric)
                                  ("ks-test", kalmogorov_smirnov);
    auto i = name2actions.find(name);
    if (i == std::end(name2actions)) {
        LOG_ERROR<<"invalid verifier name '"<<name<<"' in configuration";
        throw invalid_conf{"uknown entry for experimention " + name};
    } else if (!is_supported(i->second, mt)) {
        LOG_ERROR<<"not support for experiment "<<name<<" with model type "<<mt;
        throw invalid_conf{"experiment " + name + " not supported for model type " + actions::to_string(mt)}; 
    } else {
        LOG_DEBUG<<"creating new experiment: "<<i->first;
        return i->second;
    }
}

pipeline::config::source_policy get_input_type(json::istream& input,
        const pipeline::config::label_id& label, bool& rand)
{
    using namespace json::literals;

    if (input) {
        auto v = input ^ json::_child(input, "validation"_n);
        if (v) {
            std::string name;
            v ^ "action"_n ^ name;
            return name2input_t(v, name, label, rand);
        } else {
            LOG_ERROR<<"missing 'validation' tag in configuration";
            throw json_error{"missing 'validation' in configuration"};
        }
    }

    LOG_ERROR<<"invalid json - cannot process";
    throw json_error{"cannot process validation type,  invalid input"};
}

pipeline::config::source_policy name2input_t(json::istream& input, 
        const std::string& name, const pipeline::config::label_id& label,
        bool& rand)
{
    if (input) {
        if (name == "k-fold") {
            return kfold_action(input, label);
        } else if (name == "precent") {
            return percent_action(input, label, rand);
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
        const pipeline::config::label_id& label, bool& rand)
{
    using namespace json::literals;

    auto a = input ^ json::_child(input, "attr"_n);
    if (a) {
        unsigned int s = 0;
        a ^ "arg"_n ^ s ^ json::optional ^ "randomized"_n ^ rand;
        return pipeline::config::source_policy{percent_split{s, label}};
    }
    LOG_ERROR<<"missing attributes for precent input type";
    throw json_error{"no attribute in precent"};
}

pipeline::config::trainers::value_type
    process_action_entry(json::istream& ji, bool class_type)
{
    using namespace json::literals;
    static const std::map<std::string, action_parser> name2action = 
                    boost::assign::map_list_of("model.svm", generate_svm_action)
                    ("model.em", generate_em_action)
                    ("model.norm-bayes", generate_normal_bayes_action)
                    ("model.boosting", generate_boosting_action)
                    ("model.dtrees", generate_decision_trees_action)
                    ("model.rtrees", generate_random_tree_action)
                    ("model.log-reg", generate_logistic_regression)
                    ("model.mlp", generate_mlp_action)
                    ("model.k_nearest", generate_k_nearest_action);
    std::string n;
    ji ^ "action"_n ^ n;
    if (!ji) {
        LOG_ERROR<<"failed to read model name/type from json";
        throw json_error{"missing or invalid model name or model type"};
    }

    auto i = name2action.find(n);
    if (i == std::end(name2action)) {
        LOG_ERROR<<"unknown model type '"<<n<<"'";
        throw invalid_conf{"invalid entry " + n};
    } else {
        LOG_DEBUG<<"creating new algo: "<<i->first;
        return i->second(ji, class_type);
    }
}

pipeline::config::trainers::value_type
    generate_mlp_action(json::istream& input, bool )
{
    using namespace json::literals;

    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        int it = -1;
        double e = 0.0;
        at ^ "iterations"_n ^ it ^ "epsilon"_n ^ e;
        if (at) {
            auto f = at ^ json::_child(at, "activation-function"_n);
            auto m = at ^ json::_child(at, "train-method"_n);
            auto l = at ^ json::_child(at, "layers"_n);
            if (f && m && l) {
                double a = 0, b = 0, x = 0, y = 0;
                std::string t;
                std::vector<int> layers;
                f ^ "alpha"_n ^ a ^ "beta"_n ^ b;
                m ^ "type"_n ^ t ^ "arg-a"_n ^ x ^ "arg-b"_n ^ y;
                l ^ json::start_arr ^ layers ^ json::end_arr;
                if (layers.empty()) {
                    LOG_ERROR<<"no layers defined or failed to read layers";
                    throw invalid_conf{"missing layers for MLP model"};
                }
                return pipeline::config::trainers::value_type{
                        regression::MLP_train{
                            regression::MLP_train::model_args{
                                it, e, 
                                regression::MLP_train::model_args::activation_function{a, b},
                                regression::MLP_train::model_args::train_method{
                                    regression::MLP_train::model_args::string2method(t), x, y
                                }, layers
                            }
                        },
                        actions::REGRESSION_MODEL
                    };
            }
        }
    } 
    LOG_ERROR<<"failed to read attributes entry in json";
    throw invalid_conf{"failed to generate model MLP from configuration"};
}

pipeline::config::trainers::value_type
generate_normal_bayes_action(json::istream&, bool )
{
    return pipeline::config::trainers::value_type{classifier::normal_bayes_train{}, actions::CLASSIFIER_MODEL};
}

pipeline::config::trainers::value_type
generate_k_nearest_action(json::istream& input, bool )
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        int nc = -1;
        std::string t;
        at ^ "count"_n ^ nc ^ "type"_n ^ t;
        if (at) {
            return pipeline::config::trainers::value_type{classifier::k_nearest_train{
                classifier::k_nearest_train::model_args{classifier::k_nearest_train::model_args::string2type(t), nc}
                },
                actions::CLASSIFIER_MODEL
            };
        } else {
            LOG_ERROR<<"failed to read attributes for logistric regression";
            throw json_error{"invalid k nearst message format"};
        }
    } else {
        LOG_ERROR<<"failed to read attributes entry in json";
        throw invalid_conf{"failed to generate model K nearst from configuration"};
    }
}

pipeline::config::trainers::value_type
generate_logistic_regression(json::istream& input, bool classifier)
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        classifier::logistic_regression_train::model_args arg;    // they have the same arg type
        std::string b, r;
        at ^ "learning-rate"_n ^ arg.learning_rate ^ "iterations"_n ^ arg.iterations ^
            "min-batch-size"_n ^ arg.min_batch_size ^ "reg-level"_n ^ r ^ "batch-type"_n ^ b;
        if (at) {
            arg.regular = convert_reg(r);
            arg.train_method = convert_method(b);
            if (classifier) {
                return pipeline::config::trainers::value_type{classifier::logistic_regression_train{std::move(arg)},
                                    actions::CLASSIFIER_MODEL};
            } else {
                return pipeline::config::trainers::value_type{regression::logistic_regression_train{std::move(arg)},
                                    actions::REGRESSION_MODEL};
            }
        } else {
            LOG_ERROR<<"failed to read attributes for logistric regression";
            throw json_error{"invalid logistic regresion message format"};
        }
    }
    LOG_ERROR<<"invalid logistic regression model definition at configuration";
    throw invalid_conf{"failed to generate model logistic regression from configuration"};
}

classifier::logistic_regression_train::model_args::regularization_t 
    convert_reg(const std::string& from)
{
    return classifier::logistic_regression_train::model_args::string2reg(from);
}

classifier::logistic_regression_train::model_args::train_method_t 
    convert_method(const std::string& from)
{
    return classifier::logistic_regression_train::model_args::string2train(from);
}

pipeline::config::trainers::value_type
    generate_svm_action(json::istream& input, bool classifier)
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        std::string k;
        at ^ "kernel"_n ^ k ;
        if (at) {
            if (classifier) {
                return pipeline::config::trainers::value_type{classifier::SVM_train{
                    classifier::SVM_train::model_args{ 
                            classifier::SVM_train::model_impl::model::string2kernel(k),
                        }
                    },
                    actions::CLASSIFIER_MODEL
                };
            } else {
                return pipeline::config::trainers::value_type{regression::SVM_train{
                    regression::SVM_train::model_args{
                            regression::SVM_train::model_impl::model::string2kernel(k)
                        }
                    },
                    actions::REGRESSION_MODEL
                };
            }
        } else {
            LOG_ERROR<<"failed to read SVM variables from configuration";
            throw json_error{"failed to read SVM variables from configuration"};
        }
    }
    LOG_ERROR<<"invalid SVM model definition at configuration";
    throw invalid_conf{"failed to generate model SVM from configuration"};
}

pipeline::config::trainers::value_type
    generate_em_action(json::istream& input, bool )
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        int cc = -1, mic = -1;
        float tc = 0.f;
        std::string s;
        at ^ "clusters"_n ^ cc ^ "cluster-min"_n ^ mic ^ "temination-count"_n ^ tc ^ "shape"_n ^ s; 
        if (at) {
            return pipeline::config::trainers::value_type{
                classifier::EM_train{classifier::EM_train::model_args{cc, mic, tc, name2shape(s)}
                },
                actions::CLASSIFIER_MODEL
            };
        } else {
            LOG_ERROR<<"invalid EM model args";
            throw json_error{"failed to parse EM args"};
        }
    }
    LOG_ERROR<<"invalid EM model definition at configuration";
    throw invalid_conf{"failed to generate model EM from configuration"};
}

pipeline::config::trainers::value_type
    generate_boosting_action(json::istream& input, bool )
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        auto bs = read_dtree_args(at, "boosting");
        int wc= -1;
        double w = -1.0;
        std::string t;
        at ^ "weak-count"_n ^ wc ^ "type"_n ^ t ^ "weight-trim-rate"_n ^ w;
        if (at) {
            return pipeline::config::trainers::value_type{
                classifier::boosting_train{classifier::boosting_train::model_args{
                    classifier:: boosting_train::model_args::string2type(t), wc, w,
                                    bs.cv_fold, bs.max_categories, bs.max_depth,
                                    bs.min_samples, bs.use_surrogates,
                                    bs.prediction
                                }
                        }   ,
                actions::CLASSIFIER_MODEL
                };
        } else {
            LOG_ERROR<<"failed to read attributes for boosting";
            throw json_error{"error while processing boosting configuration"};
        }
    } else {
        LOG_ERROR<<"missing configuration values for boosting";
       throw invalid_conf{"missing confguration values for boosting"};
    } 

}

pipeline::config::trainers::value_type
    generate_decision_trees_action(json::istream& input, bool )
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        auto arg = read_dtree_args(at, "decision_trees");
        return pipeline::config::trainers::value_type{
                    classifier::decision_tree_train{arg},
                    actions::CLASSIFIER_MODEL
                };
    } else {
        LOG_ERROR<<"failed to read attributes for decition trees";
        throw json_error{"error while processing decition trees configuration"};
    }
}
       

pipeline::config::trainers::value_type
    generate_random_tree_action(json::istream& input, bool )
{
    using namespace json::literals;
    auto at = input ^ json::_child(input, "attr"_n);
    if (at) {
        double ma = 0.0;
        bool vi = false;
        auto bs = read_dtree_args(at, "random_tree");
        at ^ "regression-accuracy"_n ^ ma ^ "var-importance"_n ^ vi;
        if (at) {
            return pipeline::config::trainers::value_type{
                    classifier::random_tree_train{classifier::random_tree_train::model_args{
                                  vi, ma, bs.cv_fold, 
                                  bs.max_categories, 
                                  bs.max_depth,
                                  bs.min_samples, 
                                  bs.use_surrogates,
                                  bs.prediction
                                }
                            },
                            actions::CLASSIFIER_MODEL
                        };   
        } else {
            LOG_ERROR<<"failed to read attributes for desition trees";
            throw json_error{"error while processing random trees configuration"};
        }
    } else {
        LOG_ERROR<<"missing configuration values for random trees";
       throw invalid_conf{"missing confguration values for random trees"};
    } 
}

mlmodels::dtree_args 
    read_dtree_args(json::istream& att, const char* act_model)
{
    using namespace json::literals;

    int cvf = -1, mc = -1, md = -1,  ms = -1;
    bool us = false;
    std::string pt;
    att  ^ "flags"_n ^ pt ^ "max-categories"_n ^ mc ^ "cv-fold"_n ^ cvf ^ "max-depth"_n ^ md ^ 
        "min-samples"_n ^ ms ^ "use-surrogates"_n ^ us;
    if (att) {
        return mlmodels::dtree_args{cvf, mc, md, ms, us, mlmodels::dtree_args::string2type(pt)};
    } else {
        using namespace std::string_literals;

        LOG_ERROR<<"failed to read "<<act_model<<" attributes";
        throw json_error{"failed to read "s + act_model + " attributes"s};
    }
}

mlmodels::expectation_maximization::args::covariance_matrices_t name2shape(const std::string& sn)
{
    return mlmodels::expectation_maximization::args::string2type(sn);
}

pipeline::config::trainers process_actions(json::istream& input, bool class_type)
{
    pipeline::config::trainers actions;
    std::for_each(input.entries().begin(), input.entries().end(), [&actions, class_type](auto& entry) {
                std::string n;
                json::istream tmp(entry.second);
                actions.push_back(process_action_entry(tmp, class_type));
            }
    );
    if (actions.empty()) {
        LOG_ERROR<<"no models were defined in the configuration";
        throw invalid_conf{"no valid models were found in the configuration"};
    }
    return actions;
}
pipeline::config::trainers get_model_generators(json::istream& input, bool class_type)
{
    using namespace json::literals;

    if (input) {
        auto a = input ^ json::_child(input, "actions"_n);
        if (a) {
            //return pipeline::config::trainers{};
            return process_actions(a, class_type);
        } else {
            LOG_ERROR<<"missing attribute actions in configuration";
            throw json_error{"missing actions in configuraiton"};
        }
    }
    LOG_ERROR<<"cannot read actions - invalid JSON";
    throw json_error{"invalid json - cannot process actions"};
}
}       // end of local namespace

namespace lookup
{
    json::istream& operator ^ (json::istream& is, table_entry& te)
    {
        using namespace json::literals;
        is ^ "source"_n ^ te.source ^ "target"_n ^ te.target;
        return is;
    }
}   // end of namespace lookup

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
           return process(root ^ from); 
        }
    } else {
        return process(root ^ from);
    }
}

bool good(const pipeline::config& c) 
{
    return pipeline::good(c);
}
