#include "command_fix_actions.h"
#include "command_fix_actions_defaults.hpp"
#include "logging.h"

bool fix_action(json::istream& is, action_reader& ac)
{
     // k-neighborhood, linear, cubic
    using namespace json::literals;
    std::string algo;
    auto att = is ^ json::_child(is, "attr"_n);
    att ^ "algo"_n ^ algo;
    auto f = fix_action_generator(algo);
    LOG_DEBUG<<"creating new fixing missing value algorithm of "<<algo;
    if (f(att, ac.data)) {
        ac.action = action_reader::NA_FIX;
        return true;
    } else {
        return false;
    }
}

