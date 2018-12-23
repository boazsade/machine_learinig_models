#pragma once
#include <limits>
#include <string>
#include <iosfwd>

namespace prediction
{

struct fix_entry
{
    using value_type = float;
    using index_type = unsigned int;

    constexpr fix_entry() = default;

    constexpr fix_entry(value_type v,
                    index_type col) : value{v}, column{col}
    {
    }

    value_type value = 
        std::numeric_limits<value_type>::max();
    index_type column = 0u;
};

std::ostream& operator << (std::ostream& os, const fix_entry& fr);
std::string to_string(const fix_entry& fr);

constexpr struct _drop_entry
{
} drop_entry = _drop_entry{};

std::ostream& operator << (std::ostream& os, _drop_entry);
std::string to_string(fix_entry);

}   // end of namespace prediction

