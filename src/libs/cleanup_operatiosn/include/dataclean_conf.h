#pragma once

#include "operetional_pipline.h"
#include "action_conf.h"
#include "schema_data.h"
#include "libs/rows_drops/filterout_conf.h"
#include <boost/filesystem.hpp>
#include <iosfwd>
#include <string>
#include <vector>

struct dataclean_conf_preliminery;
// for reading the data from the json itself
// the datatype that we would be using for the actual actions
// that we would apply on the data
struct dataclean_conf
{
    using path_type = std::string;
    using actions_pipline = column_actions;
    using actions_list = std::vector<actions_pipline>;
    using id_type = actions_pipline::id_type;
    using actions_iterator = actions_list::iterator;
    using const_actions_iterator = actions_list::const_iterator;

    dataclean_conf() = default;
    dataclean_conf(dataclean_conf&&) = default;
    dataclean_conf& operator = (dataclean_conf&&) = default;
    dataclean_conf(const dataclean_conf&) = default;
    dataclean_conf& operator = (const dataclean_conf&) = default;

    dataclean_conf(dataclean_conf_preliminery&& from, const schema_type& sc);

    const actions_pipline& operator [] (const id_type& id) const; 

    path_type           source_location;
    path_type           export_file;
    actions_list        actions;
    bool                has_actions = false;
};

bool good(const dataclean_conf& dc);

std::ostream& operator << (std::ostream& os, const dataclean_conf& df);
dataclean_conf generate(dataclean_conf_preliminery&& from, const schema_type& sc);
bool empty(const dataclean_conf& dc);
std::size_t size(const dataclean_conf& dc);
dataclean_conf::actions_iterator begin(dataclean_conf& dc);
dataclean_conf::actions_iterator end(dataclean_conf& dc);
dataclean_conf::const_actions_iterator begin(const dataclean_conf& dc);
dataclean_conf::const_actions_iterator end(const dataclean_conf& dc);
inline constexpr bool has_actions(const dataclean_conf& dc) {
    return dc.has_actions;
}

