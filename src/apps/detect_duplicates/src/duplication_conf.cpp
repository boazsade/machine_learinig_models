#include "duplication_conf.h"
#include <iostream>


duplication_conf::duplication_conf(const column_id& id, resolution re, how h) :
        col_id(id), res(re), act(h)
{
}

duplication_conf::duplication_conf(const column_id& id, resolution re, const std::string& ac) :
        col_id(id), res(re), act(convert2act(ac))
{
}

duplication_conf::how duplication_conf::convert2act(const std::string& str) 
{
        static const std::string DO_FIRST = "first";
        static const std::string DO_LAST = "last";
        if (str == DO_FIRST) {
            return KEEP_FIRST;
        } else if (str == DO_LAST) {
            return KEEP_LAST;
        } else {
            return NO_DEFINED;
        }
}


std::ostream& operator << (std::ostream& os, const duplication_conf& dc)
{
    return os<<" column id: <"<<dc.col_id<<">, action: "<<
        (dc.act == duplication_conf::KEEP_LAST ? "'keep-last'" : dc.act == duplication_conf::KEEP_FIRST ?
         "'keep-first'" : "'not-set'") << ", resolution ["<<dc.res<<"]";
}

bool operator == (const duplication_conf& left, const duplication_conf& right)
{
    return left.col_id == right.col_id && 
        left.act == right.act;
}

bool operator != (const duplication_conf& left, const duplication_conf& right)
{
    return !(left == right);
}
bool operator < (const duplication_conf& left, const duplication_conf& right)
{
    return left.col_id < right.col_id;
}

