#pragma once
#include <cstddef>
#include <iosfwd>

struct input_data
{
    using index_type = std::size_t;
    using value_type = double;

    value_type value = 0.0;
    index_type row = 0;

    constexpr input_data() = default;
    constexpr input_data(value_type v, index_type r) :
        value(v), row(r)
    {
    }
};

std::ostream& operator << (std::ostream& os, const input_data& id);
std::istream& operator >> (std::istream& is, input_data& id);
