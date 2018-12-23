#pragma once
#include "prediction_row.h"
#include "prediction_output.h"
#include "functional/functional.hpp"
#include "libs/model_datatypes/model_base.h"
#include <boost/filesystem.hpp>
#include <string>
#include <iosfwd>

namespace prediction
{

struct predicate
{
    using arg_type = row;
    using result_type = output;

    using action_type = 
        poli_function<model_base::row_type, model_base::prediction_value>;

    template<typename Action>
    predicate(Action act) :
        action{std::move(act)}
    {
    }

    result_type operator ()(const arg_type& r) const;
    std::ostream& stream(std::ostream& os) const;

private:
    action_type action;
};
std::ostream& operator << (std::ostream& os, const predicate& p);
std::string to_string(const predicate& p);
predicate::result_type
    preform_action(const predicate& p, const predicate::arg_type& arg);

struct model_load
{
    using arg_type = boost::filesystem::path;
    using result_type = predicate::action_type;

    using action_type = poli_function<arg_type, result_type>;
    

    template<typename Action>
    model_load(Action act) :
        action{std::move(act)}
    {
    }

    result_type operator () (const arg_type& from) const;
    std::ostream& stream(std::ostream& os) const;

private:
    action_type action;

};
std::ostream& operator <<(std::ostream& os, const model_load&);
std::string to_string(const model_load& );

model_load::result_type
    preform_action(const model_load&, const model_load::arg_type& from);

}   // end of namesapce prediction

