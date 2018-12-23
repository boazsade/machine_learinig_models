#pragma once

#include "action_conf.h"
#include <iosfwd>
#include <string>
#include <vector>

struct dataclean_conf_preliminery 
{
    using path_type = std::string;
    using actions_pipline = preprocessed_action;
    using actions_list = std::vector<actions_pipline>;
    using id_type = actions_pipline::id_type;
    using actions_iterator = actions_list::iterator;
    using const_actions_iterator = actions_list::const_iterator;

    dataclean_conf_preliminery() = default;

    path_type           source_location;
    path_type           export_file;
    actions_list        actions;
};

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::convert_action& ca);

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::normalized_action& na);

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::drop_action& da);

void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline& act);

#if 0
void insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const dataclean_conf_preliminery::actions_pipline::map_val_action& act);
#endif
std::ostream& operator << (std::ostream& os, const dataclean_conf_preliminery& dcp);
