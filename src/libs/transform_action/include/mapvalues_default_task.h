#pragma once
#include "lookuptable_actions.h"

namespace lookup
{

tablegen_function::result_type
    preform_action(const _dont_map_op&, const tablegen_function::arg_type&);

transform_action::result_type
    preform_action(const _dont_trasform_op&, const transform_action::arg_type&);

}   // end of namespace lookup

