#pragma once
#include "libs/cleanup_operatiosn/action_conf.h"
#include "libs/json_parser/json_istream.h"

using fix_function = bool (*) (json::istream&, preprocessed_action&);
fix_function fix_action_generator(const std::string& name);
