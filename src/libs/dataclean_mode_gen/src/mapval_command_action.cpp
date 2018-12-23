#if 0
#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "details/mapval_command_action.h"
#include "libs/cleanup_operatiosn/map_values_conf.h"
#include "libs/cleanup_operatiosn/mapvalues_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"


bool process_mapval_action(json::istream& is, action_reader& ac)
{
    using namespace json::literals;

    if (is) {
        map_values_conf cf;
        auto att = is ^ json::_child(is, "attr"_n);
        if (att) {
            att ^ "colid"_n ^ cf.column;        
            if (att) {
                ac.data.map_action() = preprocessed_action::map_val_action{cf};
                ac.action = action_reader::MAP_VALUES;
                ac.data.id() = cf.column;
                return true;
            } else {
                LOG_ERROR<<"failed to read column id for map value action from json";
                throw json_error{"failed to read column id for map value action from json"};
                return false;
            }
        } else {
            LOG_ERROR<<"failed to read column id for map values action!!";
            throw json_error{"failed to read column id for action map values"};
            return false;
        }
    } else {
        LOG_ERROR<<"cannot read from json input invalid state";
        throw json_error{"failed to parse map values action"};
        return false;
    }


}
#endif
