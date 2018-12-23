#pragma once
#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "libs/json_parser/json_istream.h"
#include <string>

normalized_data::conf_type 
    make_znorm_action(json::istream& is, const std::string& id);
normalized_data::conf_type 
    make_max_action(json::istream& is, const std::string& id);
normalized_data::conf_type 
    make_minmax_action(json::istream& is, const std::string& id);

