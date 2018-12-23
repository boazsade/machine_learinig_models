#include "command_fix_actions_defaults.h"
#include "command_processing_fix_all.hpp"
#include "command_processing_fix_const.hpp"
#include "logging.h"
#include <boost/assign/list_of.hpp>
#include <string>
#include <map>

namespace
{
    using name2actions = std::map<std::string, fix_function>;

    const name2actions& action_mapper();

#ifdef SUPPORT_FOR_MODEL_USE
    const name2actions& action_mapper()
    {
        static const name2actions action_proc = 
            boost::assign::map_list_of("const", create_const_fix);
        return action_proc;
    }
#elif defined(SUPPORT_FOR_MODEL_GENERATION)
    const name2actions& action_mapper()
    {
        static const name2actions action_proc = 
            boost::assign::map_list_of("kneighborhood", create_kneighbhood)
                                  ("linear", create_linear)
                                  ("const", create_const_fix)
                                  ("cubical", create_cubic);
        return action_proc;
    }
#else
#   error "you must define either SUPPORT_FOR_MODEL_GENERATION or SUPPORT_FOR_MODEL_USE"
#endif  // SUPPORT_FOR_MODEL_GENERATION
    bool error_case_create_func(json::istream&, preprocessed_action&) {
        return false;
    }
}   // end of local namespace


fix_function fix_action_generator(const std::string& name)
{
    auto i = action_mapper().find(name);
    if (i == std::end(action_mapper())) {
        LOG_ERROR<<"the name of this action is known '"<<name<<"' - return default action function that return false";
        return error_case_create_func;
    } else {
        return i->second;
    }
    
}

