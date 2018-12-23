#include "apps/model_prediction/command_processing.h"
#include "apps/model_prediction/schema_read.h"
#include "libs/system/exceptions.h"
#include "libs/json_parser/json_istream.h"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "libs/model_prediction/prediction_norm.h"
#include "libs/model_prediction/prediction_norm_task.h"
#include "libs/classifier_models_actions/classifers_use_task.h"
#include "libs/regression_models_actions/regressionsmodels_use_task.h"
#include "libs/classifier_models_actions/classifer_predict_interface.h"
#include "libs/regression_models_actions/regression_model_use.h"
#include "logging.h"
#include <boost/filesystem.hpp>
#include <boost/variant.hpp>
#include <map>
#include <functional>
#include <tuple>

namespace
{

struct norm_action
{
    using action_type = prediction::normalized::action_list::value_type;
    using value_type = 
        boost::variant<bool, action_type>;

    norm_action() = default;
    explicit norm_action(const schema_mapper& sc) :
        mapper{sc}, value{false}
    {
    }

    const schema_mapper& mapper;
    value_type value;
};

struct fix_action
{
    using action_type =prediction::verify_fix::actions_list::value_type; 
    using value_type = 
        boost::variant<bool, action_type>;
    explicit fix_action(const schema_mapper& m) :
        mapper{m}
    {
    }

    const schema_mapper& mapper;
    value_type value;

};

struct is_normalize :
    boost::static_visitor<bool>
{
    bool operator () (bool f) const
    {
        return f;
    }

    bool operator ()(const norm_action::action_type& ) const
    {
        return true;
    }
};

struct get_norm :
    boost::static_visitor<const norm_action::action_type&>
{
    const norm_action::action_type operator () (bool) const
    {
        static const norm_action::action_type& err = norm_action::action_type{};
        return err;
    }

    const norm_action::action_type& operator () (const norm_action::action_type& v) const
    {
        return v;
    }
};

struct is_fix_action :
    boost::static_visitor<bool>
{
    bool operator () (bool f) const
    {
        return f;
    }

    bool operator ()(const fix_action::action_type& ) const
    {
        return true;
    }
};

struct return_fix_action :
    boost::static_visitor<const fix_action::action_type&>
{
    const fix_action::action_type& operator () (bool) const
    {
        static const fix_action::action_type& err = fix_action::action_type{};
        return err;
    }

    const fix_action::action_type& operator () (const fix_action::action_type& v) const
    {
        return v;
    }
};

using norm_parser_f = norm_action::action_type(*)(json::istream&, int);

json::istream& operator ^ (json::istream& ji, norm_action& na);
json::istream& operator ^ (json::istream& ji, fix_action& fa);
prediction_operation process_cli(json::istream ji, const schema_mapper& );
prediction::normalized 
    process_norm_actions(json::istream& ji, const schema_mapper& scheme);
prediction::normalized 
    process_norms(json::istream& ji, const schema_mapper& scheme);
std::tuple<prediction::load_model, prediction::load_model::arg_type>
    process_model(json::istream& ji);
prediction::verify_fix
    process_fix_actions(json::istream& ji, const schema_mapper& scheme);
prediction::verify_fix
    process_fixs(json::istream& ji, const schema_mapper& scheme);
boost::filesystem::path read_data_source(json::istream& ji);
fix_action::action_type 
    get_fix_action(json::istream&, const schema_mapper&);
norm_action::action_type
    get_norm_action(json::istream& is, const schema_mapper&);
norm_action::action_type parse_max(json::istream&, int);
norm_action::action_type parse_minmax(json::istream&, int);
norm_action::action_type parse_zrom(json::istream&, int);
prediction::load_model make_reg_model(const std::string& name, const std::string& path);
prediction::load_model make_class_model(const std::string& name, const std::string& path);
///////////////////////////////////////////////////////////


prediction_operation process_cli(json::istream ji, const schema_mapper& scheme)
{
    auto norm{process_norm_actions(ji, scheme)};
    auto fix{process_fix_actions(ji, scheme)};
    auto load{process_model(ji)};
    auto path{source_file_name(read_data_source(ji))};
    return prediction_operation{prediction::read_row{scheme},
                            std::move(fix), std::move(norm),
                            std::move(std::get<0>(load)), std::move(path),
                            std::get<1>(load)
                        };
}   

boost::filesystem::path read_data_source(json::istream& ji)
{
    using namespace json::literals;

    if (ji) {
        auto ds = ji ^ json::_child(ji, "datasource"_n);
        if (ds) {
            std::string p;
            ds ^ "path"_n ^ p;
            if (ds) {
                return boost::filesystem::path{p};
            }
        }
    }
    LOG_ERROR<<"failed to read path from configuration";
    throw json_error{"failed to read data source location from configuration"};
}

prediction::verify_fix
    process_fix_actions(json::istream& ji, const schema_mapper& scheme)
{
    using namespace json::literals;
    if (ji) {
        auto a = ji ^ json::optional ^ json::_child(ji, "actions"_n);
        if (a) {
            LOG_INFO<<"start reading normalization actions from configuration";
            return process_fixs(a, scheme);
        } else {
            LOG_WARNING<<"no actions were read from configuration";
            return prediction::verify_fix{};    // nothing in the configuration
        }
    }
    LOG_ERROR<<"cannot read fix actions from configuration";
    throw json_error{"faild to read fix actions from configuration"};
}

prediction::verify_fix
    process_fixs(json::istream& ji, const schema_mapper& scheme)
{
    using namespace json::literals;

    prediction::verify_fix::actions_list actions;
    for (auto& entry : ji.entries()) {
        json::istream tmp(entry.second);
        fix_action act{scheme};
        tmp ^ ""_n ^ act;
        if (boost::apply_visitor(is_fix_action{}, act.value)) {
            actions.push_back(boost::apply_visitor(return_fix_action{}, act.value));
        }
    }
    return prediction::verify_fix{std::begin(actions), std::end(actions)};
}

prediction::normalized 
    process_norm_actions(json::istream& ji, const schema_mapper& scheme)
{
    // we need to pass on every entry and check if this is a normalization 
    // entry, and if so, we would generate a normalization action for it
    using namespace json::literals;
    if (ji) {
        auto a = ji ^ json::optional ^ json::_child(ji, "actions"_n);
        if (a) {
            LOG_INFO<<"start read normalization actions";
            return process_norms(a, scheme);
        } else {
            LOG_WARNING<<"no actions were read from configuration";
            return prediction::normalized{};    // nothing in the configuration
        }
    }
    LOG_ERROR<<"cannot read normalization actions from configuration";
    throw json_error{"faild to read normalization actions from configuration"};

}

prediction::normalized 
    process_norms(json::istream& ji, const schema_mapper& scheme)
{
    using namespace json::literals;

    prediction::normalized::action_list actions;
    for (auto& entry : ji.entries()) {
        json::istream tmp(entry.second);
        norm_action act{scheme};
        tmp ^ ""_n ^ act;
        if (boost::apply_visitor(is_normalize{}, act.value)) {
            actions.push_back(boost::apply_visitor(get_norm{}, act.value));
        }
    }
    return prediction::normalized{std::begin(actions), std::end(actions)};
}

json::istream& operator ^ (json::istream& is, fix_action& fa)
{
    using namespace json::literals;

    std::string an;
    is ^ "action"_n ^ an;
    LOG_DEBUG<<"processing action "<<an;
    if (an == "fix.missing") {
        fa.value = get_fix_action(is, fa.mapper);
    } else {
        fa.value = false;
    }
    return is;
}

json::istream& operator ^ (json::istream& is, norm_action& na)
{
    using namespace json::literals;

    std::string an;
    is ^ "action"_n ^ an;
    LOG_DEBUG<<"processing action "<<an;
    if (an == "normalized") {
        na.value = get_norm_action(is, na.mapper);
    } else {
        na.value = false;
    }
    return is;
}

fix_action::action_type 
    get_fix_action(json::istream& is, const schema_mapper& mapper)
{
    using namespace json::literals;
    auto att = is ^ json::_child(is, "attr"_n);
    if (att) {
        std::string cid, act;
        att ^ "algo"_n ^ act ^ "colid"_n ^ cid;
        if (att) {
            LOG_INFO<<"read action fix "<<act<<" for column "<<cid;
            if (act == "const") {
                auto col = mapper[cid];
                if (col == schema_mapper::invalid) {
                    LOG_ERROR<<"invalid column id passed - "<<cid<<" for fix action";
                    throw invalid_conf{"no such column in input file " + cid};
                }
                double val = -1;
                att ^ "value"_n ^ val;
                if (att) {
                    return fix_action::action_type{prediction::column_number{col}, val};
                } else {
                    LOG_ERROR<<"failed to read fix value from configuration for column "<<cid;
                    throw invalid_conf{"failed to read fix value for missing values for column " + cid};
                }
            } else {
                LOG_ERROR<<"invalid action for fix was passed '"<<act<<"' for column "<<cid;
                throw invalid_conf{"invalid fix action " + act + " for column " + cid};
            }
        } else {
            LOG_ERROR<<"failed to read configuration for column and algorithm for fix action";
            throw json_error{"fail to read column and fix action from configuration"};
        }
    }
    LOG_ERROR<<"failed to read fix action attributes from configuration";
    throw json_error{"fail to read fix action attributes from configuration"};
}

norm_action::action_type
    get_norm_action(json::istream& is, const schema_mapper& sm)
{
    using namespace json::literals;
    using namespace std::string_literals;

    static const std::map<std::string, norm_parser_f> parsing_m{
        {"znorm"s, parse_zrom},
        {"min-max"s, parse_minmax},
        {"max"s, parse_max}
    };

    auto att = is ^ json::_child(is, "attr"_n);
    if (att) {
        std::string cid, act;
        att ^ "algo"_n ^ act ^ "colid"_n ^ cid;
        if (att) {
            auto col = sm[cid];
            if (col == schema_mapper::invalid) {
                throw invalid_conf{"no such column in input file " + cid};
            }
            auto i = parsing_m.find(act);
            if (i != std::end(parsing_m)) {
                return i->second(att, col);
            } else {
                throw invalid_conf{"invalid nomalization action given " + act};
            }
        }
    } 
    LOG_ERROR<<"failed to read attribute value for nomalization action";
    throw json_error{"failed to read attribute value for normalization action"};
}

norm_action::action_type parse_max(json::istream& is, int cid)
{
    using namespace json::literals;
    LOG_INFO<<"creating max norm action for column "<<cid;
    double min = std::numeric_limits<double>::max(), max = std::numeric_limits<double>::max();
    // try to read paramters that are optional so we would know what type of action to create
    is ^ "minval"_n ^ min ^ "maxval"_n ^ max;
    if (is) {
        LOG_INFO<<"creating max norm with min value "<<min<<" and max value "<<max<<" on id "<<cid;
        return prediction::normalized::action_list::value_type{max_normalization_stats{cid, min, max}};
    }
    throw json_error{"failed to read input paramters for max normalization"};
}

norm_action::action_type parse_minmax(json::istream& is, int cid)
{
    using namespace json::literals;
    LOG_INFO<<"creating min/max norm action for column "<<cid;
    double min = -1, max = -1;
    // try to read paramters that are optional so we would know what type of action to create
    is ^ "min"_n ^ min ^ "max"_n ^ max;
    if (is) {
        double minVal = std::numeric_limits<double>::max(), maxVal = std::numeric_limits<double>::max();
        is ^ "minval"_n ^ minVal ^ "maxval"_n ^ maxVal;
        if (is) {
            LOG_INFO<<"creating max norm with min value "<<min<<" and max value "<<max<<" and range min "<<minVal<<", max "<<maxVal<<" on id "<<cid;
            return prediction::normalized::action_list::value_type{minmax_normalization_stats{cid, min, max, minVal, maxVal}};
        } else {
            LOG_ERROR<<"failed to read min value and max value for normalization";
            throw json_error{"failed to read min and max values for min max normalization"};
        }
    } else {
        throw json_error{"failed to read min and max values for min max normalization"};
    }
}

norm_action::action_type parse_zrom(json::istream& is, int cid)
{
    using namespace json::literals;
    LOG_INFO<<"creating z norm action for column "<<cid;
        // check if we have the parameters or not, if not then this is the default, else based on stats
    double m = std::numeric_limits<double>::max(), std = std::numeric_limits<double>::max();
    is ^ "mean"_n ^ m ^ "STD"_n ^ std;
    if (is) {
        LOG_INFO<<"create z norm action that uses pre computed mean "<<m<<" and STD "<<std<<" for column "<<cid;
        return prediction::normalized::action_list::value_type{z_normaliztion_conf_stats{cid, m, std}};
    }
    throw json_error{"failed to read mean and STD values for normalization"};
}

std::tuple<prediction::load_model, prediction::load_model::arg_type>
    process_model(json::istream& ji)
{
    using namespace json::literals;
    
    auto m = ji ^ json::_child(ji, "model"_n);
    if (m) {
        std::string n, t, p;
        m ^ "name"_n ^ n ^ "type"_n ^ t ^ "path"_n ^ p;
        if (m) {
            if (t == "classifier") {
                return std::make_tuple(make_class_model(n, p), p);
            } else {
                return std::make_tuple(make_reg_model(n, p), p);
            }
        }
    }
    throw json_error{"failed to read model arguments from configuration"};
}

prediction::load_model make_class_model(const std::string& name, const std::string& path)
{
    using namespace std::string_literals;
    using factory_t = std::function<prediction::load_model()>;

    static const auto make_svm = []() { return prediction::load_model{classifier::SVM_load{}}; };
    static const auto make_em = []() { return prediction::load_model{classifier::EM_load{}}; };
    static const auto make_nb = []() { return prediction::load_model{classifier::normal_bayes_load{}}; };
    static const auto make_rt = []() { return prediction::load_model{classifier::random_tree_load{}}; };
    static const auto make_kn = []() { return prediction::load_model{classifier::k_nearest_load{}}; };
    static const auto make_r = []() { return prediction::load_model{classifier::logistic_regression_load{}}; };
    static const auto make_dt = []() { return prediction::load_model{classifier::decision_tree_load{}}; };
    static const auto make_bt = []() { return prediction::load_model{classifier::boosting_load{}}; };
    static const std::map<std::string, factory_t> load_p{
        {"model.svm"s, factory_t{make_svm}},
        {"model.em"s, factory_t{make_em}},
        {"model.boosting"s, factory_t{make_bt}},
        {"model.dtrees"s, factory_t{make_dt}},
        {"model.rtrees"s, factory_t{make_rt}},
        {"model.norm-bayes"s, factory_t{make_nb}},
        {"model.k_nearest"s, factory_t{make_kn}},
        {"model.log-reg"s, factory_t{make_r}}
    };
    auto i = load_p.find(name);
    if (i != std::end(load_p)) {
        LOG_INFO<<"makeing new classifier model action for model '"<<name<<"' that would be loaded from path "<<path;
        return i->second();
    } 
    LOG_ERROR<<"no such model "<<name<<" cannot create";
    throw invalid_conf{"no such model " + name};
}

prediction::load_model make_reg_model(const std::string& name, const std::string& path)
{
    using namespace std::string_literals;
    using factory_t = std::function<prediction::load_model()>;

    static const auto make_svm = []() { return prediction::load_model{regression::SVM_load{}}; };
    static const auto make_lr = []() { return prediction::load_model{regression::logistic_regression_load{}}; };
    static const auto make_mlp = []() { return prediction::load_model{regression::MLP_load{}}; };
    static const std::map<std::string, factory_t> load_p{
        {"model.svm"s, factory_t{make_svm}},
        {"model.mlp"s, factory_t{make_mlp}},
        {"model.log-reg"s, factory_t{make_lr}}
    };
    auto i = load_p.find(name);
    if (i != std::end(load_p)) {
        LOG_INFO<<"makeing new regression model action for model '"<<name<<"' that would be loaded from path "<<path;
        return i->second();
    } 
    LOG_ERROR<<"no such model "<<name<<" cannot create";
    throw invalid_conf{"no such model " + name};
}

}   // end of local namespace

///////////////////////////////////////////////////////////////////////////////
//
prediction_operation process_command(const std::string& cmd,
                                    const schema_mapper& scheme)
{
    if (cmd.empty()) {
        LOG_ERROR<<"could not process CLI - empty args given";
        throw invalid_conf{"empty args given"};
    } else if (scheme.empty()) {
        LOG_ERROR<<"called with an empty schema - cannot process";
        throw invalid_conf{"failed to generate scheme from input file"};
    } else {
        static constexpr auto max_path = 256u;
        json::istream_root root;
        if (cmd.size() < max_path) {
            boost::filesystem::path p{cmd};
            if (boost::filesystem::exists(p)) {
                return process_cli(root ^ p, scheme);
            } else {
                LOG_INFO<<"trying to process from command line";
                return process_cli(root ^ cmd, scheme);
            }
        } else {
            return process_cli(root ^ cmd, scheme);
        }
    }
}
