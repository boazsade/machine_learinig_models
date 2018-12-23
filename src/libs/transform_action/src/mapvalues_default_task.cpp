#include "libs/transform_action/mapvalues_default_task.h"
namespace lookup
{
tablegen_function::result_type
    preform_action(const _dont_map_op&, const tablegen_function::arg_type&)
{
    return make_trasform_func(dont_trasform_op);
}

transform_action::result_type
    preform_action(const _dont_trasform_op&, const transform_action::arg_type& args)
{
    return args;
}

uniqued_function::result_type 
    preform_action(const _dont_trasform_op&, const uniqued_function::arg_type&)
{
    return -1;  // nothing
}
}   // end of namespace lookup

