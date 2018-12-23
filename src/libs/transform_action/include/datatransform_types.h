#pragma once
#include <string>
#include <iosfwd>

std::string to_string (const int& a);
namespace lookup
{

struct table;

constexpr struct _dont_map_op{} dont_map_op = _dont_map_op{};
constexpr struct _dont_trasform_op{} dont_trasform_op = _dont_trasform_op{};
constexpr struct _uniqued_op {} uniqued_op = _uniqued_op{};

std::string to_string(const _dont_map_op& du);
std::ostream& operator << (std::ostream& os, _dont_map_op);

std::string to_string(const _dont_trasform_op& du);
std::ostream& operator << (std::ostream& os, _dont_trasform_op);
std::string to_string(const _uniqued_op& du);
std::ostream& operator << (std::ostream& os, _uniqued_op);

std::string to_string(const table& tbl);

}   // end of namespace lookup

