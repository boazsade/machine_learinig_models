#pragma once
#include "libs/model_datatypes/model_types.h"
#include <string>
#include <iosfwd>

namespace lookup
{

struct map_values_conf
{
    using id_type = std::string;

    map_values_conf() = default;
    map_values_conf(const id_type& id,
            actions::model_type t);

    bool classifier() const
    {
        return type == actions::CLASSIFIER_MODEL;
    }

    id_type column;
    actions::model_type type = actions::UNKOWN_MODEL_TYPE;
};

std::string to_string(const map_values_conf&);
std::ostream& operator << (std::ostream& os, const map_values_conf&);

}   // end of lookup namespace 

