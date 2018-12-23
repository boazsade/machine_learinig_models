#include "libs/cleanup_operatiosn/operetional_pipline.h"
#include "libs/cleanup_operatiosn/normalized_task.h"

normalized_data::normalized_data(const conf_type& ct) : 
    configuration(ct)
{
}

normalized_data::normalized_data() :
    configuration(normalized_nothing)
{
}

normalized_data::result_type normalized_data::operator () (const arg_type& args) const
{
    return preform_action(configuration, args);
}


void normalized_data::print(std::ostream& os) const
{
    os<<configuration;
}

const normalized_data::conf_type& normalized_data::config() const
{
    return configuration;
}

std::ostream& operator << (std::ostream& os, const normalized_data& du)
{
    du.print(os);
    return os;
}

std::string to_string(const normalized_data& du)
{
    return to_string(du.config());
}

normalized_data::result_type preform_action(
        const normalized_data& action, normalized_data::arg_type arg)
{
    return action(arg);
}


