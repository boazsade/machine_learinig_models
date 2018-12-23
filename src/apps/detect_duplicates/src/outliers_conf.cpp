#include "outliers_conf.h"
#include <iostream>


outliers_conf::outliers_conf(const column_id& id) :
   col_id(id)
{
}


bool operator == (const outliers_conf& left, const outliers_conf& right)
{
    return left.col_id == right.col_id;
}

bool operator != (const outliers_conf& left, const outliers_conf& right)
{
    return !(left == right);
}

bool operator < (const outliers_conf& left, const outliers_conf& right)
{
    return left.col_id < right.col_id;
}


std::ostream& operator << (std::ostream& os, const outliers_conf& oc)
{
    return os<<" column ["<<oc.col_id<<"]";
}

