#pragma once
#include "command_action_reader.hpp"
#include "libs/json_parser/json_istream.h"

bool fix_action(json::istream& is, action_reader& ac);
