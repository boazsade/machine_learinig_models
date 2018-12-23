#include "libs/model_prediction/prediction_flow.h"
#include "libs/model_prediction/prediction_row.h"
#include "utils/missing_values.hpp"
#include "logging.h"
#include <boost/variant.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <iostream>
#include <sstream>

namespace prediction
{

namespace 
{

struct run_prediction :
    boost::static_visitor<apply_model::result_type>
{
    run_prediction(const predicate& p) :
        predictor{p}
    {
    }

    apply_model::result_type operator () (const output& o) const
    {
        return o;
    }

    apply_model::result_type operator () (const row& r) const
    {
        LOG_DEBUG<<"running prediction for row "<<index(r);
        return preform_action(predictor, r);
    }

private:
    const predicate& predictor;
};

struct normalized_data : 
    boost::static_visitor<normalized::result_type>

{
    normalized_data(const normalized::action_list& al) :
        actions{al}
    {
    }

    normalized::result_type operator () (const output& o) const
    {
        return normalized::result_type{o};
    }

    normalized::result_type operator () (row r) const
    {
        row result{r};
        LOG_DEBUG<<"normalizing data for row "<<index(r);
        for (const auto& act : actions) {
            auto corret_val = preform_action(act, r);
            r[column(act).value] = corret_val;
        }
        return normalized::result_type{std::move(r)};
    }

    const normalized::action_list& actions;
};

///////////////////////////////////////////////////////////////////////////////
//
struct fix_missing_vals :
    boost::static_visitor<verify_fix::result_type>
{
    fix_missing_vals(const verify_fix::actions_list& al) :
        actions{al}
    {
    }


    verify_fix::result_type operator () (output o) const
    {
        return verify_fix::result_type{o};
    }

    verify_fix::result_type operator () (row r) const
    {
        column_number from{0}, to = (column_number)r.size();
        while (from != to) {
            from = find(r, from, to);
            if (from != to) {
                LOG_DEBUG<<"need to fix invalid entry for row "<<index(r);
                auto f = match_column(from);
                if (f == actions.end()) {   // we don't have a fix for it
                    LOG_INFO<<"no fixing action for row "<<index(r)<<" this would be removed";
                    return verify_fix::result_type{output{index(r), status::REMOVED}};
                } else {
                    LOG_INFO<<"fixing invalid value for row "<<index(r);
                    replace(r, *f);
                }
            }
        }
        return verify_fix::result_type{std::move(r)};
    }

private:
    using iterator = verify_fix::actions_list::const_iterator;

    column_number find(const row& r, column_number start, column_number end) const
    {
        boost::counting_iterator<column_number::value_type> from{start.value},
                                                            to{end.value};
        //for (; start != end; ++start) {
        auto i = std::find_if(from, to, [&r](auto col) {
                    return missing_value(r[col]);
                }
            );
        return i == to ? end : column_number{*i};
    }

    iterator match_column(column_number at) const
    {
        return std::find_if(std::begin(actions), std::end(actions), [at](const auto& act) {
                                return act.column == at;
                        }
                );
    }

    const verify_fix::actions_list& actions;
};

}   // end of local namespace

///////////////////////////////////////////////////////////////////////////////
//
void verify_fix::assign(const actions_list::value_type act) 
{
    actions.push_back(std::move(act));
}

void normalized::assign(action_list::value_type new_entry) 
{
    actions.push_back(std::move(new_entry));
}


verify_fix::result_type verify_fix::operator () (arg_type input) const
{
    input = boost::apply_visitor(fix_missing_vals{actions}, input.value);
    return input;
}

std::ostream& operator << (std::ostream& os, const verify_fix& vf)
{
    return vf.stream(os);
}

std::ostream& verify_fix::stream(std::ostream& os) const
{
    os<<"fix missing actions ";
    if (actions.empty()) {
        return os<<"no actions";
    } else {
        std::copy(std::begin(actions), std::end(actions), std::ostream_iterator<actions_list::value_type>(os, "\n"));
        return os;
    }
}

std::string to_string(const verify_fix& vf)
{
    std::stringstream sp;
    sp<<vf;
    return sp.str();
}

verify_fix::result_type apply(const verify_fix& fix, verify_fix::arg_type what)
{
    return fix(what);
}

///////////////////////////////////////////////////////////////////////////////
//
normalized::result_type normalized::operator () (arg_type arg) const
{
    return boost::apply_visitor(normalized_data{actions}, arg.value);
}

std::ostream& normalized::stream(std::ostream& os) const
{
    os<<"normalized actions:\n";
    for (const auto& ac : actions) {
        os<<"action: "<<ac<<"\n";
    }
    return os;
}

std::string to_string(const normalized& n)
{
    std::ostringstream p;
    p<<n;
    return p.str();
}

std::ostream& operator << (std::ostream& os, const normalized& n)
{
    return n.stream(os);
}

normalized::result_type norm(const normalized& with, const normalized::arg_type& what)
{
    return with(what);
}

///////////////////////////////////////////////////////////////////////////////
//
read_row::result_type read_row::operator()(arg_type& input) const
{
    try {
        auto r = read(input, input_layout);
        if (r.empty()) {
            return result_type{output{invalid_row(), status::END_OF_FILE}};
        } else {
            return result_type{r};
        }
    } catch (const std::runtime_error& e) {
        LOG_ERROR<<"error while trying to read row from input: '"<<e.what()<<"'";
        return result_type{output{invalid_row(), status::INVALID}};
    }
}

read_row::result_type read(const read_row& with,
                        read_row::arg_type& input)
{
    return with(input);
}

read_row::read_row(const schema_mapper& sc) : input_layout{sc}
{
}

///////////////////////////////////////////////////////////////////////////////


apply_model::result_type predict(const apply_model& with, const apply_model::arg_type& on)
{
    return with(on);
}

apply_model::apply_model(predicate p) : predictor{std::move(p)}
{
}

apply_model::result_type apply_model::operator () (const arg_type& arg) const
{
    run_prediction r{predictor}; 
    return boost::apply_visitor(r, arg.value);
}

std::ostream& operator << (std::ostream& os, const apply_model& am)
{
    return am.stream(os);
}

std::ostream& apply_model::stream(std::ostream& os) const
{
    return os<<"apply model action "<<predictor;
}

///////////////////////////////////////////////////////////////////////////////

load_model::result_type load_model::operator () (const arg_type& arg) const
{
    return preform_action(action, arg);
}

std::ostream& load_model::stream(std::ostream& os) const
{
    return os<<"load model: "<<action;
}
load_model::result_type
    load(const load_model& lm, const load_model::arg_type& arg)
{
    return lm(arg);
}

std::ostream& operator << (std::ostream& os, const load_model& lm)
{
    return lm.stream(os);
}

std::string to_string(const load_model& lm)
{
    std::ostringstream p;
    p<<lm;
    return p.str();
}

std::ostream& operator << (std::ostream& os, const read_row&)
{
    return os<<"read operation";
}
}   // end of namespace predition

