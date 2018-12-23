#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/base_normalized_task.h"
#include "details/norm_command_action_impl.hpp"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "libs/cleanup_operatiosn/normalized_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"


normalized_data::conf_type 
    make_znorm_action(json::istream& is, const std::string& id)
{
        // check if we have the parameters or not, if not then this is the default, else based on stats
    LOG_INFO<<"creating default z norm action that would compute the mean and STD internally for id "<<id;
    if (is) {
        return normalized_data::conf_type{z_normaliztion_conf{0}};
    } 
    throw json_error{"failed to read z rnomalization from json configuration"};
}

normalized_data::conf_type 
    make_max_action(json::istream& is, const std::string& id)
{
    using namespace json::literals;
    if (is) {
        LOG_INFO<<"create normal max norm operation on id "<<id;
        return normalized_data::conf_type{max_normalization{0}};
    } 
    throw json_error{"failed to read max normalization attribute from json"};
}

normalized_data::conf_type 
    make_minmax_action(json::istream& is, const std::string& id)
{
    using namespace json::literals;

    double min = -1, max = -1;
    // try to read paramters that are optional so we would know what type of action to create
    is ^ "min"_n ^ min ^ "max"_n ^ max;
    if (is) {
        LOG_INFO<<"create normal max norm operation on id "<<id<<" min "<<min<<" max "<<max;
        return normalized_data::conf_type{minmax_normalization{0, min, max}};
    } else {
        throw json_error{"failed to read min and max values for min max normalization"};
    }
}

