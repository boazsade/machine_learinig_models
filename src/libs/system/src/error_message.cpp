#include "libs/system/error_message.h"
#include "libs/json_parser/json_ostream.h"
#include <iostream>

json::ostream& operator ^ (json::ostream& js, const error_report& err) {
    using namespace json::literals;

    js ^ "error"_n ^ err.msg ^ "msg"_n ^ err.details;
    return js;
}


std::ostream& operator << (std::ostream& os, const error_report& er)
{
    return os<<"title: '"<<er.msg<<"' details: '"<<er.details<<"'";
}

std::string to_json(const error_report& er)
{
    using namespace json::literals;

    json::output_stream root;
    auto parser = root ^ json::open;
    auto err = parser ^ "errors"_s;
    err ^ er ^ json::_end;
    auto msg = root ^ json::str_cast;
    return msg;
}
