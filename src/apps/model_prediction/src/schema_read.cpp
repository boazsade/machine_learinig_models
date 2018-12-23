#include "apps/model_prediction/schema_read.h"
#include "libs/sources_handling/schema_type.h"
#include "libs/sources_handling/schema_read.hpp"
#include "libs/system/exceptions.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "logging.h"
#include <fstream>
#include <sstream>

namespace 
{

schema_type load_schema(std::ifstream& from)
{
    return sr::read_schema(from, " \t", [](auto token) { return token; });
}

boost::property_tree::ptree
    json_read(const std::string& from)
{
    std::istringstream src{from};
    boost::property_tree::ptree root;
    boost::property_tree::read_json(src, root);
    return root;
}

boost::property_tree::ptree
    load_json(const std::string& from)
{
    static const auto max_file_name = 256u;
    if (not from.empty()) {
        if (from.size() > max_file_name) {
            return json_read(from);    
        }     
        boost::filesystem::path p{from};
        if (boost::filesystem::exists(p)) { // assume that this is a path to read from
            boost::property_tree::ptree root;
            boost::property_tree::read_json(from, root);
            return root;
        } else {
            return json_read(from);
        }
    }
    throw invalid_conf{"empty source for configuration was given"};
}

}   // end of namespace


boost::filesystem::path source_file_name(boost::filesystem::path base_path)
{
        base_path /= "numeric_table";
        base_path /= "numeric_table";
        return base_path;
}

schema_mapper read_schema(const std::string& path)
{
    LOG_INFO<<"trying to read model schema with configuration of size "<<path.size();
    boost::property_tree::ptree root = load_json(path);
    boost::filesystem::path sp = root.get<std::string>("datasource.path");
    if (not sp.empty()) {
        sp = source_file_name(sp);
        // we need to read the file, but we need to paste the actual location of the file as this is a directory name    
        if (boost::filesystem::exists(sp)) {
            std::ifstream source{sp.string()};
            auto sc = load_schema(source);
            LOG_INFO<<"read "<<sc.size()<<" columns from "<<sp;
            return schema_mapper(std::begin(sc), std::end(sc)); 
        } else {
            LOG_ERROR<<"file not found "<<sp.string();
        }
    } else {
        LOG_ERROR<<"empty path given to sources location";
    }
    return schema_mapper{};
}
