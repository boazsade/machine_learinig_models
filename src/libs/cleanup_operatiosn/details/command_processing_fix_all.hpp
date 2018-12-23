#pragma once
#include "libs/cleanup_operatiosn/action_conf.h"
#include "libs/json_parser/json_istream.h"
bool create_kneighbhood(json::istream&, preprocessed_action&);
bool create_linear(json::istream&, preprocessed_action&);
bool create_cubic(json::istream&, preprocessed_action&);
