#pragma once

#include "duplication_conf.h"
#include "outliers_conf.h"
#include <boost/filesystem.hpp>
#include <vector>

struct app_args {
    using duplication_actions = std::vector<duplication_conf>;
    using outliers_actions = std::vector<outliers_conf>;

    boost::filesystem::path input_path;
    duplication_actions     duplications;
    outliers_actions        outliers;
};

bool parse_command(const std::string& input, app_args& args);

std::ostream& operator << (std::ostream& os, const app_args& args);


