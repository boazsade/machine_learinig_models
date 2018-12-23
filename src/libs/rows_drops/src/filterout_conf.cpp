#include "libs/rows_drops/filterout_conf.h"
#include <iostream>
#include <iterator>
#include <algorithm>


filterout_conf::filterout_conf(const column_id& id) :
    column(id)
{
}


bool valid(const filterout_conf& fc)
{
    return !(fc.column.empty() && fc.to_remove.empty());
}


std::ostream& operator << (std::ostream& os, const filterout_conf& fc)
{
    os<<"filter out row action";
    if (valid(fc)) {
        os<<" id: '"<<fc.column<<"', filterout: [";
        std::copy(std::begin(fc.to_remove), std::end(fc.to_remove),
                std::ostream_iterator<filterout_conf::row_numbers::value_type>(os, ", "));
        os<<"]";
    } else {
        os<<" invalid";
    }
    return os;
}

bool operator == (const filterout_conf& left, const filterout_conf& right)
{
    return left.column == right.column;
}

bool operator != (const filterout_conf& left, const filterout_conf& right)
{
    return !(left == right);
}

bool operator < (const filterout_conf& left, const filterout_conf& right)
{
    return left.column < right.column;
}

bool operator == (const filterout_conf::column_id& left, const filterout_conf& right)
{
    return left == right.column;
}

bool operator != (const filterout_conf::column_id& left, const filterout_conf& right)
{
    return !(left == right);
}

bool operator < (const filterout_conf::column_id& left, const filterout_conf& right)
{
    return left < right.column;
}
bool operator == (const filterout_conf& left, const filterout_conf::column_id& right)
{
    return left.column == right;
}

bool operator != (const filterout_conf& left, const filterout_conf::column_id& right)
{
    return !(left == right);
}

bool operator < (const filterout_conf& left, const filterout_conf::column_id& right)
{
    return left.column < right;
}

std::string to_string(const filterout_conf&)
{
    return "filterout_conf";
}
