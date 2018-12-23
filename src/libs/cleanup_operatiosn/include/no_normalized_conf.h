#pragma once
#include <string>
#include <iosfwd>

constexpr struct _normalized_nothing {} normalized_nothing = _normalized_nothing{};  // default
constexpr struct _t_normalization {} t_normalization = _t_normalization{};
std::ostream& operator << (std::ostream& os, _normalized_nothing);
std::ostream& operator << (std::ostream& os, const _t_normalization& nb);
std::string to_string(const _t_normalization& n);
std::string to_string(const _normalized_nothing& du);


