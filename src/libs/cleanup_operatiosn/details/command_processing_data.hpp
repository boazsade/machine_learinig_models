#include "libs/cleanup_operatiosn/command_processing_data.h"
#include "logging.h"
#include <iostream>
#include <iterator>
#include <algorithm>

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline& act)
{
    LOG_DEBUG<<"saving new action entry '"<<id<<"' with actions "<<act;
    auto i = std::find(std::begin(to.actions), std::end(to.actions), id);
    if (i == std::end(to.actions)) {
        LOG_DEBUG<<id<<": this is a new action";
        to.actions.push_back(act);
    } else {    // we have this action, we need to update
        LOG_DEBUG<<id<<": updates action "<<*i;
        *i = act;
    }
}
#if 0
void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::map_val_action& act)
{
    LOG_DEBUG<<"saving new action entry '"<<id<<"' with actions "<<act;
    auto i = std::find(std::begin(to.actions), std::end(to.actions), id);
    if (i == std::end(to.actions)) {
        static const dataclean_conf_preliminery::actions_pipline::convert_action ca = 
            dataclean_conf_preliminery::actions_pipline::convert_action{};
        static const dataclean_conf_preliminery::actions_pipline::normalized_action na = 
            dataclean_conf_preliminery::actions_pipline::normalized_action{};
        static const dataclean_conf_preliminery::actions_pipline::drop_action da = 
            dataclean_conf_preliminery::actions_pipline::drop_action{};
        to.actions.emplace_back(id, da, ca, na, act);
    } else {
        i->map_action() = act;
    }
}
#endif
std::ostream& operator << (std::ostream& os, const dataclean_conf_preliminery& dcp)
{
    os<<"sources path: '"<<dcp.source_location<<"', export path: '"<<dcp.export_file<<"\n";
    std::copy(std::begin(dcp.actions), std::end(dcp.actions), 
            std::ostream_iterator<dataclean_conf_preliminery::actions_list::value_type>(os, "\n"));
    return os;
}

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::convert_action& ca)
{
    LOG_DEBUG<<"saving new action entry '"<<id<<"' with actions "<<ca;
    auto i = std::find(std::begin(to.actions), std::end(to.actions), id);
    if (i == std::end(to.actions)) {
        static const dataclean_conf_preliminery::actions_pipline::drop_action da = 
            dataclean_conf_preliminery::actions_pipline::drop_action{};
        static const dataclean_conf_preliminery::actions_pipline::normalized_action na = 
            dataclean_conf_preliminery::actions_pipline::normalized_action{};
        to.actions.emplace_back(id, da, ca, na);
    } else {
        i->fixes() = ca;
    }
}

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::normalized_action& na)
{
    LOG_DEBUG<<"saving new action entry '"<<id<<"' with actions "<<na;
    auto i = std::find(std::begin(to.actions), std::end(to.actions), id);
    if (i == std::end(to.actions)) {
        static const dataclean_conf_preliminery::actions_pipline::drop_action da = 
            dataclean_conf_preliminery::actions_pipline::drop_action{};
        static const dataclean_conf_preliminery::actions_pipline::convert_action ca = 
            dataclean_conf_preliminery::actions_pipline::convert_action{};
        to.actions.emplace_back(id, da, ca, na);
    } else {
        i->normalization() = na;
    }
}

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::drop_action& da)
{
    LOG_DEBUG<<"saving new action entry '"<<id<<"' with actions "<<da;
    auto i = std::find(std::begin(to.actions), std::end(to.actions), id);
    if (i == std::end(to.actions)) {
        static const dataclean_conf_preliminery::actions_pipline::convert_action ca = 
            dataclean_conf_preliminery::actions_pipline::convert_action{};
        static const dataclean_conf_preliminery::actions_pipline::normalized_action na = 
            dataclean_conf_preliminery::actions_pipline::normalized_action{};
        to.actions.emplace_back(id, da, ca, na);
    } else {
        i->drop() = da;
    }
}

