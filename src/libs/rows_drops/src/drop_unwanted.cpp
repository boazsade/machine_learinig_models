#include "libs/rows_drops/drop_unwanted.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include <iostream>

drop_unwanted::drop_unwanted() :
    configuration(dont_drop)
{
}

drop_unwanted::drop_unwanted(const conf_type& ct) : 
    configuration(ct)
{
}

drop_unwanted::result_type drop_unwanted::operator () (const arg_type& args) const
{
    return preform_action(configuration, args);
}

drop_unwanted::result_type preform_action(
        const drop_unwanted& action, drop_unwanted::arg_type arg)
{
    return action(arg);
}

std::ostream& operator << (std::ostream& os, const _dont_drop& )
{
    return os<<"no drop action is required";
}

std::string to_string(const _dont_drop&)
{
    return "no drop";
}

std::string to_string(const drop_unwanted& du)
{
    return to_string(du.config());
}

drop_unwanted& drop_unwanted::operator=(const conf_type& c) 
{
    configuration = c;
    return *this;
}

void drop_unwanted::print(std::ostream& os) const
{
    os<<configuration;
}

std::ostream& operator << (std::ostream& os, const drop_unwanted& du)
{
    du.print(os);
    return os;
}
