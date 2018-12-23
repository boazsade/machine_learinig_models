#pragma once
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include <iosfwd>

bool operator == (const source_data& left, const source_data& right);
bool operator != (const source_data& left, const source_data& right);
bool operator == (const input_data& left, const input_data& right);
bool operator != (const input_data& left, const input_data& right);
std::ostream& operator << (std::ostream& os, const source_data& sd);
std::ostream& operator << (std::ostream& os, source_data::iterator );
//std::ostream& operator << (std::ostream& os, const input_data& sd);
source_data make_test_data(unsigned int size, const source_data::id_type& id);
source_data from_double_array(const double* from, const double* to, const std::string& name);
