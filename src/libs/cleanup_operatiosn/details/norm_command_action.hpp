#include "norm_command_action.h"
#include "norm_command_action_impl.hpp"
#include "libs/cleanup_operatiosn/normalized_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"
#include <boost/assign/list_of.hpp>
#include <string>
#include <map>

bool create_znorm(json::istream&, preprocessed_action&);
bool create_minmax(json::istream&, preprocessed_action&);
bool create_tnorm(json::istream&, preprocessed_action&);
bool create_max(json::istream&, preprocessed_action&);
using normalized_function = bool (*)(json::istream&, preprocessed_action&);

bool normalized_action(json::istream& is, action_reader& ac)
{
    using namespace json::literals;
    // min-max, znorm, tnorm, max
    using name2actions = std::map<std::string, normalized_function>;
    static const name2actions action_proc = 
        boost::assign::map_list_of("znorm", create_znorm)
                                  ("tnorm", create_tnorm)
                                  ("max", create_max)
                                  ("min-max", create_minmax);
    std::string algo;
    auto att = is ^ json::_child(is, "attr"_n);
    att ^ "algo"_n ^ algo;
    auto i = action_proc.find(algo);
    if (i == std::end(action_proc)) {
        LOG_ERROR<<"expecting missing normalization call, but this is not supported "<<algo;
        return false;
    } else {
        LOG_DEBUG<<"creating new normaliztion value algorithm of "<<algo;
        auto f = i->second;
        if (f(att, ac.data)) {
            ac.action = action_reader::NORM_FIX;
            return true;
        } else {
            return false;
        }
    }
}


bool create_minmax(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;
    
    std::string id;
    is ^ "colid"_n ^ id;
    if (is) {
        act.id() = id;
        act.normalization() = make_minmax_action(is, id);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for min max";
        return false;
    }
}

bool create_tnorm(json::istream& , preprocessed_action& )
{
     LOG_WARNING<<"t norm is not supported!!";
     throw invalid_conf{"t norm is not supported"};
     return false;

}

bool create_znorm(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;

    std::string id;
    is ^ "colid"_n ^ id;
    if (is) {
        act.id() = id;
        act.normalization() = make_znorm_action(is, id);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for znorm";
        return false;
    }
}

bool create_max(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;

    std::string id;
    is ^ "colid"_n ^ id;
    if (is) {
        act.id() = id;
        act.normalization() = make_max_action(is, id);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for max";
        return false;
    }

}


