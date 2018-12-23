#pragma once
#include "libs/sources_handling/map_schema.h"
#include <boost/filesystem.hpp>
#include <string>

boost::filesystem::path 
    source_file_name(boost::filesystem::path base_path);

schema_mapper read_schema(const std::string& path);

