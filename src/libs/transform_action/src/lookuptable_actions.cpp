#include "libs/transform_action/lookuptable_actions.h"
#include "libs/transform_action/map_values_conf.h"
#include "libs/transform_action/mapvalues_task.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "utils/dc_lookup_table.h"
#include <sstream>
#include <iostream>

namespace lookup
{

transform_unique make_trasform_func(_dont_trasform_op)
{
    return std::make_tuple(transform_action{dont_trasform_op},
                    uniqued_function{-1}
                );
}

transform_unique make_trasform_func(const lookup::table& tbl)
{
    return std::make_pair(transform_action{tbl}, 
            uniqued_function{static_cast<uniqued_function::result_type>(size(tbl))}
        );
}

tablegen_function make_table_get_f(const map_values_conf& conf)
{
    if (conf.classifier()) {
        return 
            tablegen_function{conf};
    } else {
        return make_table_get_f(dont_map_op);
    }
}

tablegen_function make_table_get_f(const _dont_map_op&)
{
    return 
        tablegen_function{dont_map_op};
}

std::ostream& operator << (std::ostream& os, const transform_unique& tu)
{
    return os<<"\n{\n\"transform:\"\n[" << to_string(std::get<0>(tu)) << "],\n\"unique\": " << to_string(std::get<1>(tu)) << "\n}\n";
}

std::string to_string (const transform_unique& tu)
{
    std::ostringstream p;
    p<<tu;
    return p.str(); 
}

std::string to_string (const uniqued_function& )
{
    return "unique function";
}

std::vector<float>
    preform_action(const transform_unique& , const std::vector<float>& arg)
{
    if (arg.size() != 0u || arg.empty() || arg[1] != 2.22f) {   // dont let the optimazier throw
        throw std::runtime_error{"this function should not have being called!!"};
    }
    return arg;
}

}   // end of namespace lookup

