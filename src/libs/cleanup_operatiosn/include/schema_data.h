#pragma once
#include "libs/sources_handling/schema_type.h"
#include <string>
#include <vector>
#include <iosfwd>

schema_type get_schema(std::istream& from);
