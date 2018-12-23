#pragma once
#include "libs/cleanup_operatiosn/action_conf.h"
#include <string>

struct action_reader
{
    enum action_type {
        DROP, NA_FIX, NORM_FIX, MAP_VALUES, NOT_SET
    };

    action_reader() = default;

    preprocessed_action data;
    action_type         action = NOT_SET;
};

const std::string& column_id(const action_reader& ar)
{
    return ar.data.id();
}


