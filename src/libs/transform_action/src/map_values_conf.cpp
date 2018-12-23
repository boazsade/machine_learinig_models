#include "libs/transform_action/map_values_conf.h"
#include <iostream>

namespace lookup
{

map_values_conf::map_values_conf(const id_type& id,
        actions::model_type t) :
    column{id}, type{t}
{
}

std::string to_string(const map_values_conf& conf)
{
    static const std::string label = " type: ";
    static const std::string lc = label + " classfier";
    static const std::string rc = label + " regression";

    return "map-value-[#" + conf.column + "]" + (conf.classifier() ? lc : rc);
}

std::ostream& operator << (std::ostream& os, const map_values_conf& conf)
{
    return os<<to_string(conf);
}

}   // end of namespace lookup

