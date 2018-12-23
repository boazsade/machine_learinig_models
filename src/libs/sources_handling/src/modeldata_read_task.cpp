#include "libs/sources_handling/modeldata_read_task.h"
#include "libs/system/exceptions.h"
#include "logging.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace 
{

modeldata_read_base::result_type
    handle_read(const modeldata_read_base::arg_type& from);

modeldata_read_base::result_type
    handle_read(const modeldata_read_base::arg_type& from)
{
    if (!boost::filesystem::exists(from)) {
        LOG_ERROR<<"invalid configuration - file not found "<<from;
        throw invalid_conf{"path not found: " + from.string()};
    }
    std::ifstream input{from.string()};
    if (!input) {
        LOG_ERROR<<"invlid configuration - cannot open "<<from<<" for reading";
        throw invalid_conf{"failed to open source file " + from.string()};
    }
    // read the schema -
    auto sh = read_schema(input);
    if (sh.empty()) {
        LOG_ERROR<<"failed to read schema information from "<<from;
        throw invalid_conf{"failed to read schema information from source at " + from.string()};
    } else {
        LOG_INFO<<"successfully read schema from "<<from<<" with "<<sh.size()<<" entries";
    }
    // now read the input
    return classifier::read(input, sh); 
}

}   // end of local namespace

std::string to_string(modeldata_read_t )
{
    return "model data read";
}
std::ostream& operator << (std::ostream& os, modeldata_read_t )
{
    return os<<to_string(modeldata_read);
}

std::string to_string(rand_modeldata_read_t )
{
    return "randomized model data read";
}

std::ostream& operator << (std::ostream& os, rand_modeldata_read_t )
{
    return os<<to_string(rand_modeldata_read);
}

modeldata_read_t::result_type
    preform_action(modeldata_read_t, const modeldata_read_t::arg_type& p)
{
    LOG_INFO<<"reading into data from "<<p<<" by "<<modeldata_read;
    return handle_read(p);
}

rand_modeldata_read_t::result_type
    preform_action(rand_modeldata_read_t, const modeldata_read_t::arg_type& p)
{
    LOG_INFO<<"reading into data from "<<p<<" by "<<rand_modeldata_read;
    return classifier::randomalized(handle_read(p));
}
