#pragma once
#include "map_values_conf.h"
#include "lookuptable_actions.h"
#include "functional/functional.hpp"
#include "libs/sources_handling/row_data_input.h"
#include "libs/ml_models/general_types.h"
#include <iosfwd>
#include <string>

namespace lookup
{

struct map_value_op
{
    using conf_type = tablegen_function;
    using arg_type = conf_type::arg_type;
    using result_type = conf_type::result_type;;
    
    map_value_op();
    map_value_op(const map_values_conf& ct);    // create internally action that does something
    map_value_op(_dont_map_op);                 // create empty operation

    result_type operator () (const arg_type& args) const;
    void print(std::ostream& os) const;
    const conf_type& config() const;
private:
    conf_type configuration;
};

map_value_op::result_type preform_action(
        const map_value_op& action, const map_value_op::arg_type& arg);
std::ostream& operator << (std::ostream& os, const map_value_op& du);
std::string to_string(const map_value_op& du);

}   // end of namespace lookup

