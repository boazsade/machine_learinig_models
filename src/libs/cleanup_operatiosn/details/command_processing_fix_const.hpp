#pragma once
#include "command_action_reader.hpp"
#include "libs/cleanup_operatiosn/fillinvalid_conf.h"
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "libs/json_parser/json_istream.h"
#include "logging.h"

bool create_const_fix(json::istream& is, preprocessed_action& pa)
{
    using namespace json::literals;
    const_value_fix conf;

    is ^ "colid"_n ^ conf.column ^ "value"_n ^ conf.value;
    auto r = is ^ json::_child(is, "rows"_n);
    r  ^ conf.invalid_rows;
    if (is) {
        pa.id() = conf.column;
        std::sort(std::begin(conf.invalid_rows), std::end(conf.invalid_rows));
        pa.fixes() = preprocessed_action::convert_action::conf_type(conf);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for const fix action";
        return false;
    }
}
