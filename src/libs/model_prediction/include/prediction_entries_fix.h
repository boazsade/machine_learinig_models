#pragma once
#include "prediction_datatypes.h"
#include "libs/cleanup_operatiosn/constfill_conf.h"
#include <string>
#include <iosfwd>

namespace prediction
{

struct row;

struct fix_missing :
    basic_contval_fix
{

    constexpr fix_missing() = default;

    constexpr fix_missing(column_number cid, double v) :
            basic_contval_fix{v}, column{cid}
    {
    }

    column_number column;
};

std::ostream& operator << (std::ostream& os, const fix_missing& fm);
std::string to_string(const fix_missing& fm);

void replace(row& r, const fix_missing& fixer);

}   // end of namespace prediction

