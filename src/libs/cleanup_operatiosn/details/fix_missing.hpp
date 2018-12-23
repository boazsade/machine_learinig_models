#pragma once
#include "libs/cleanup_operatiosn/fix_missing.h"


fix_missing::fix_missing() :
    configuration(fix_nothing)
{
}

fix_missing::fix_missing(const conf_type& ct) :
    configuration(ct)
{
}


std::string to_string(const fix_missing& du)
{
    return to_string(du.config());
}

const drop_unwanted::conf_type& drop_unwanted::config() const
{
    return configuration;
}

fix_missing::result_type fix_missing::operator () (const arg_type& args) const
{
    return preform_action(configuration, args);
}

fix_missing::result_type preform_action(
        const fix_missing& action, fix_missing::arg_type arg)
{
    return action(arg);
}

std::ostream& operator << (std::ostream& os, const fix_missing& du)
{
    du.print(os);
    return os;
}
const fix_missing::conf_type& fix_missing::config() const
{
    return configuration;
}

std::ostream& operator << (std::ostream& os, _fix_nothing)
{
    return os<<"apply no fix action";
}

void fix_missing::print(std::ostream& os) const
{
    os<<configuration;
}

std::string to_string(const _fix_nothing& )
{
    return "don't fix missing";
}

