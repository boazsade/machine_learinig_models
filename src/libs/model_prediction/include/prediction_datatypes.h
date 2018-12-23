#pragma once
#include "libs/ml_models/general_types.h"
#include "utils/missing_values.hpp"
#include <iosfwd>
#include <string>

namespace prediction
{

struct row_number
{
    using value_type = unsigned int;

    explicit constexpr row_number(value_type v = invalid()) :
        value{v}
    {
    }

    static constexpr value_type invalid() {
        return missing_value<value_type>();
    }

    constexpr row_number& operator = (value_type v) {
        value = v;
        return *this;
    }

    value_type value = invalid();
};

inline constexpr bool operator == (row_number l, row_number r) {
    return l.value == r.value;
}

inline constexpr bool operator != (row_number l, row_number r) {
    return !(l == r);
}

inline constexpr bool operator < (row_number l, row_number r) {
    return l.value < r.value;
}

inline constexpr row_number invalid_row() {
    return row_number{};
}

inline constexpr bool invalid_row(row_number rn) {
    return rn == invalid_row();
}

std::ostream& operator << (std::ostream& os, row_number r);
std::string to_string(row_number r);

///////////////////////////////////////////////////////////////////////////////

struct column_number 
{
    using value_type = int;

    static constexpr value_type invalid() {
        return -1;
    }

    constexpr explicit column_number(value_type v = invalid()) :
            value{v}
    {
    }

    constexpr column_number& operator = (value_type v) {
        value = v;
        return *this;
    }

    value_type value = invalid();
};

inline constexpr bool operator == (column_number l, column_number r) {
    return l.value == r.value;
}

inline constexpr bool operator != (column_number l, column_number r) {
    return !(l == r);
}

inline constexpr bool operator < (column_number l, column_number r) {
    return l.value < r.value;
}

inline constexpr column_number invalid_column() {
    return column_number{};
}

inline constexpr bool invalid_column(column_number rn) {
    return rn == invalid_column();
}

std::ostream& operator << (std::ostream& os, column_number r);
std::string to_string(column_number r);

///////////////////////////////////////////////////////////////////////////////



using row_data_t = mlmodels::class_data;
using data_type = row_data_t::value_type;

}   // end of namespace prediction

