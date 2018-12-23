#pragma once
#include "map_values_conf.h"
#include "lookuptable_actions.h"
#include "utils/dc_lookup_table.h"

namespace lookup
{
tablegen_function::result_type preform_action(
        const map_values_conf& action, const tablegen_function::arg_type& arg);

transform_action::result_type
    preform_action(const table&, const transform_action::arg_type&);

uniqued_function::result_type 
    preform_action(uniqued_function::result_type val,  const uniqued_function::arg_type&);

}   // end of namespace lookup

