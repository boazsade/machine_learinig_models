#pragma once
#include "libs/cleanup_operatiosn/action_conf.h"
#include "libs/json_parser/json_istream.h"

bool create_const_fix(json::istream&, preprocessed_action&);
