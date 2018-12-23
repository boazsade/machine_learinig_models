#include "libs/cleanup_operatiosn/fillinvalid_baseconf.h"
#include "utils/algorithms.hpp"

std::ostream& operator << (std::ostream& os, const fillinvalid_base& fb)
{
    os<<"id: "<<fb.column<<", invalid rows: [";
    out_range(os, fb.invalid_rows, ", ");
    return os<<"]";
}

bool operator == (const fillinvalid_base& left, const fillinvalid_base& right)
{
    return left.column == right.column;
}

bool operator != (const fillinvalid_base& left, const fillinvalid_base& right)
{
    return !(left == right);
}

bool operator < (const fillinvalid_base& left, const fillinvalid_base& right)
{
    return left.column < right.column;
}

bool operator == (fillinvalid_base::column_id left, const fillinvalid_base& right)
{
    return left == right.column;
}

bool operator != (fillinvalid_base::column_id left, const fillinvalid_base& right)
{
    return !(left == right);
}

bool operator < (fillinvalid_base::column_id left, const fillinvalid_base& right)
{
    return left < right.column;
}

bool operator == (const fillinvalid_base& left, fillinvalid_base::column_id right)
{
    return left.column == right;
}
bool operator != (const fillinvalid_base& left, fillinvalid_base::column_id right)
{
    return !(left == right);
}

bool operator < (const fillinvalid_base& left, fillinvalid_base::column_id right)
{
    return left.column < right;
}

fillinvalid_base::fillinvalid_base(column_id id) :
    basic_fillinvalid{id}
{
}

std::string to_string(basic_fillinvalid bf)
{
    using namespace std::string_literals;

    return "fill inavid column index "s + std::to_string(bf.column);
}

std::ostream& operator << (std::ostream& os, const basic_fillinvalid& bf)
{
    return os<<to_string(bf);
}

std::ostream& operator << (std::ostream& os, _default_fill)
{
    return os<<to_string(default_fill);
}

