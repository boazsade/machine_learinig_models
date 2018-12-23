#include "libs/cleanup_operatiosn/action_conf.h"
#include <iostream>

const preprocessed_action::id_type& id(const preprocessed_action& apl)
{
    return apl.id();
}

const column_actions::id_type& id(const column_actions& ca)
{
    return ca.id();
}

boost::filesystem::path data_location(const column_actions& ca)
{
    boost::filesystem::path p{id(ca)};
    p /= id(ca);
    return p;
}

bool operator == (const preprocessed_action& left, const preprocessed_action::id_type& right)
{
    return id(left) == right;
}

bool operator != (const preprocessed_action& left, const preprocessed_action::id_type& right)
{
    return !(left == right);
}

bool operator == (const preprocessed_action::id_type& left, const preprocessed_action& right)
{
    return id(right) == left;
}

bool operator != (const preprocessed_action::id_type& left, const preprocessed_action& right)
{
    return !(left == right);
}

bool operator == (const column_actions& left, const column_actions::id_type& right)
{
    return id(left) == right;
}

bool operator != (const column_actions& left, const column_actions::id_type& right)
{
    return !(left == right);
}

bool operator == (const column_actions::id_type& left, const column_actions& right)
{
    return id(right) == left;
}

bool operator != (const column_actions::id_type& left, const column_actions& right)
{
    return !(left == right);
}

std::ostream& operator << (std::ostream& os, const column_actions& ca)
{
    return os<<"id '"<<id(ca)<<"', drop: "<<ca.drop()<<", fix missing: "<<ca.fixes()<<", normalization: "<<ca.normalization();
}

std::ostream& operator << (std::ostream& os, const preprocessed_action& ca)
{
    return os<<"id '"<<id(ca)<<"', drop: "<<ca.drop()<<", fix missing: "<<ca.fixes()<<", normalization: "<<ca.normalization();
}

