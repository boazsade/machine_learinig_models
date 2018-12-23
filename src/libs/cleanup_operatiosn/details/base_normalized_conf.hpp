#include "libs/cleanup_operatiosn/base_normalized_conf.h"
#include <iostream>

normaliztion_base::normaliztion_base(column_id id) :
    column(id)
{
}

std::ostream& operator << (std::ostream& os, const normaliztion_base& nb)
{
    return os<<to_string(nb);
}

bool operator == (const normaliztion_base& left, const normaliztion_base& right)
{
    return left.column == right.column;
}

bool operator != (const normaliztion_base& left, const normaliztion_base& right)
{
    return !(left == right);
}

bool operator < (const normaliztion_base& left, const normaliztion_base& right)
{
    return left.column < right.column;
}

bool operator == (normaliztion_base::column_id left, const normaliztion_base& right)
{
    return left == right.column;
}

bool operator < (const normaliztion_base::column_id left, const normaliztion_base& right)
{
    return left < right.column;
}

bool operator != (const normaliztion_base::column_id left, const normaliztion_base& right)
{
    return !(left == right);
}

bool operator == (const normaliztion_base& left, normaliztion_base::column_id right)
{
    return left.column == right;
}

bool operator != (const normaliztion_base& left, normaliztion_base::column_id right)
{
    return !(left == right);
}

bool operator < (const normaliztion_base& left, normaliztion_base::column_id right)
{
    return left.column < right;
}

std::string to_string(const normaliztion_base& nb)
{
    return "normaliztion_base@" + std::to_string(nb.column);
}

std::ostream& operator << (std::ostream& os, const z_normaliztion_conf& nb)
{
    return os<<to_string(nb);
}

std::ostream& operator << (std::ostream& os, const max_normalization& nb)
{
    return os<<to_string(nb);
}



std::ostream& operator << (std::ostream& os, const minmax_normalization& nb)
{
    return os<<to_string(nb);
}

minmax_normalization::minmax_normalization(column_id id, 
            double a, double b) :
    normaliztion_base(id), min(a), max(b)
{

}


std::string to_string(const minmax_normalization& mn)
{
    using namespace std::string_literals;
    return "minmax_normalization"s + to_string(static_cast<const normaliztion_base&>(mn)) + 
        "min "s + std::to_string(mn.min) + ", max "s + std::to_string(mn.max);
}

std::string to_string(const max_normalization& mn)
{
    using namespace std::string_literals;
    return "max_normalization"s + to_string(static_cast<const normaliztion_base&>(mn));
}

std::string to_string(const z_normaliztion_conf& mn)
{
    using namespace std::string_literals;
    return "z_normaliztion_conf" + to_string(static_cast<const normaliztion_base&>(mn));
}

