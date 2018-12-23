#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "details/command_processing_fix_all.hpp"
#include "details/command_processing_fix_const.h"
#include "libs/cleanup_operatiosn/fillinvalid_conf.h"
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"

bool create_kneighbhood(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;
    kneighborhood_fix conf{0};

    std::string col;
    is ^ "colid"_n ^ col ^ "count"_n ^ conf.n_count;
    if (!is) {
        throw json_error{"failed to read column id for kneighbhood action"};
    }
    auto r = is ^ json::_child(is, "rows"_n);
    r  ^ conf.invalid_rows;
    if (r) {
        act.id() = col;
        std::sort(std::begin(conf.invalid_rows), std::end(conf.invalid_rows));
        act.fixes() = preprocessed_action::convert_action::conf_type(conf);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for kneighbhood";
        throw json_error{"failed to read kneighbhood rows"};
        return false;
    }
}

bool create_linear(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;
    linear_fix conf{0};

    std::string col;
    is ^ "colid"_n ^ col ;
    if (!is) {
        throw json_error{"failed to read column id for linear action"};
    }
    auto r = is ^ json::_child(is, "rows"_n);
    r  ^ conf.invalid_rows;
    if (r) {
        act.id() = col;
        std::sort(std::begin(conf.invalid_rows), std::end(conf.invalid_rows));
        act.fixes() = preprocessed_action::convert_action::conf_type(conf);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for linear";
        throw json_error{"failed to read linear rows"};
        return false;
    }
}

bool create_cubic(json::istream& is, preprocessed_action& act)
{
    using namespace json::literals;
    cubical_fix conf{0};

    if (!is) {
        throw json_error{"cannot read any attributes for cubical action"};
    }
    std::string col;
    is ^ "colid"_n ^ col ;
    if (!is) {
        throw json_error{"failed to read column id for cubical action"};
    }
    auto r = is ^ json::_child(is, "rows"_n);
    r  ^ conf.invalid_rows;
    if (r) {
        act.id() = col;
        std::sort(std::begin(conf.invalid_rows), std::end(conf.invalid_rows));
        act.fixes() = preprocessed_action::convert_action::conf_type(conf);
        return true;
    } else {
        LOG_WARNING<<"failed to parse input for cubic";
        throw json_error{"failed to read cubic rows"};
        return false;
    }
}
