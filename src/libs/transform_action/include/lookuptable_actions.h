#pragma once
#include "datatransform_types.h"
#include "functional/functional.hpp"
#include "libs/sources_handling/row_data_input.h"
#include <tuple>
#include <iosfwd>
#include <string>

namespace lookup
{

struct map_values_conf;

using transform_action  = poli_function<mlmodels::class_data,
                                         mlmodels::class_data>;

using uniqued_function = poli_function<_uniqued_op, int>;

using transform_unique = std::tuple<transform_action,
                                    uniqued_function>;

using tablegen_function = poli_function<classifier::read_result,
                                        transform_unique>;


std::string to_string (const uniqued_function&);
transform_unique make_trasform_func(_dont_trasform_op);
transform_unique make_trasform_func(const lookup::table&);
tablegen_function make_table_get_f(const map_values_conf&);
tablegen_function make_table_get_f(const _dont_map_op&);

std::ostream& operator << (std::ostream& os, const transform_unique&);
std::string to_string (const transform_unique&);

std::vector<float>
    preform_action(const transform_unique&, const std::vector<float>&);
}   // end of namepspace lookup

