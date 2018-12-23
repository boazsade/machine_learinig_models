#include "libs/transform_action/map_values_op.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/mapvalues_task.h"
#include <iostream>
#include <sstream>

namespace lookup
{

map_value_op::map_value_op() :
    configuration{dont_map_op}
{
}

map_value_op::map_value_op(const map_values_conf& ct) :
        configuration{make_table_get_f(ct)}
{
}

map_value_op::map_value_op(_dont_map_op) :
        configuration{dont_map_op}
{
}


map_value_op::result_type map_value_op::operator () (const arg_type& args) const
{
    return preform_action(config(), args);
}

void map_value_op::print(std::ostream& os) const
{
    os<<"map values: "<<config();
}

const map_value_op::conf_type& map_value_op::config() const
{
    return configuration;
}

map_value_op::result_type preform_action(
        const map_value_op& action, const map_value_op::arg_type& arg)
{
    return action(arg);
}

std::ostream& operator << (std::ostream& os, const map_value_op& du)
{
    du.print(os);
    return os;
}

std::string to_string(const map_value_op& du)
{
    std::ostringstream p;
    p<<du;
    return p.str();
}

}   // end of namespace lookup
