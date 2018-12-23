#include "apps/run_experiments/exceptions.h"

invalid_input::invalid_input(const std::string& p) :
        details{"invalid input: " + p}
{
}

const std::string& invalid_input::what() const
{
    return details;
}


invalid_conf::invalid_conf(const std::string& p) :
    details{"invalid configuration given: " + p}
{
}

const std::string& invalid_conf::what() const
{
    return details;
}

json_error::json_error(const std::string& p) :
    details{"JSON error: " + p}
{
}

const std::string& json_error::what() const
{
    return details;
}

