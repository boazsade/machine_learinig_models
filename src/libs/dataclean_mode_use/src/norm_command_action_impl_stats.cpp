#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif
#include "details/norm_command_action_impl.hpp"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "libs/cleanup_operatiosn/normalized_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"


normalized_data::conf_type 
    make_znorm_action(json::istream& is, const std::string& id)
{
    using namespace json::literals;
        // check if we have the parameters or not, if not then this is the default, else based on stats
    if (is) {
    double m = std::numeric_limits<double>::max(), std = std::numeric_limits<double>::max();
        is ^ "mean"_n ^ m ^ "STD"_n ^ std;
        if (is) {
            LOG_INFO<<"create z norm action that uses pre computed mean "<<m<<" and STD "<<std<<" for column "<<id;
            return normalized_data::conf_type{z_normaliztion_conf_stats{0, m, std}};
        }
    }
    throw json_error{"failed to read mean and STD values for normalization"};
}

normalized_data::conf_type 
    make_max_action(json::istream& is, const std::string& id)
{
    using namespace json::literals;

    if (is) {
        double min = std::numeric_limits<double>::max(), max = std::numeric_limits<double>::max();
    // try to read paramters that are optional so we would know what type of action to create
        is ^ "minval"_n ^ min ^ "maxval"_n ^ max;
        if (is) {
            LOG_INFO<<"creating max norm with min value "<<min<<" and max value "<<max<<" on id "<<id;
            return normalized_data::conf_type{max_normalization_stats{0, min, max}};
        }
    }
    throw json_error{"failed to read input paramters for max normalization"};
}

normalized_data::conf_type 
    make_minmax_action(json::istream& is, const std::string& id)
{
    using namespace json::literals;

    double min = -1, max = -1;
    // try to read paramters that are optional so we would know what type of action to create
    is ^ "min"_n ^ min ^ "max"_n ^ max;
    if (is) {
        double minVal = std::numeric_limits<double>::max(), maxVal = std::numeric_limits<double>::max();
        is ^ "minval"_n ^ minVal ^ "maxval"_n ^ maxVal;
        if (is) {
            LOG_INFO<<"creating max norm with min value "<<min<<" and max value "<<max<<" and range min "<<minVal<<", max "<<maxVal<<" on id "<<id;
            return normalized_data::conf_type{minmax_normalization_stats{0, min, max, minVal, maxVal}};
        } else {
            LOG_ERROR<<"failed to read min value and max value for normalization";
            throw json_error{"failed to read min and max values for min max normalization"};
        }
    } else {
        throw json_error{"failed to read min and max values for min max normalization"};
    }
}

