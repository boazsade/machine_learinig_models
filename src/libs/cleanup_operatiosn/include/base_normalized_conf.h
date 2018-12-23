#pragma once

#include <iosfwd>

struct normaliztion_base
{
    using column_id = int;

    normaliztion_base() = default;
    explicit normaliztion_base(column_id id);

    column_id column = -1;
};

std::ostream& operator << (std::ostream& os, const normaliztion_base& nb);
bool operator == (const normaliztion_base& left, const normaliztion_base& right);
bool operator != (const normaliztion_base& left, const normaliztion_base& right);
bool operator < (const normaliztion_base& left, const normaliztion_base& right);
bool operator == (normaliztion_base::column_id left, const normaliztion_base& right);
bool operator < (normaliztion_base::column_id left, const normaliztion_base& right);
bool operator != (normaliztion_base::column_id left, const normaliztion_base& right);
bool operator == (const normaliztion_base& left, normaliztion_base::column_id right);
bool operator != (const normaliztion_base& left, normaliztion_base::column_id right);
bool operator < (const normaliztion_base& left, normaliztion_base::column_id right);
std::string to_string(const normaliztion_base& n);

struct z_normaliztion_conf : normaliztion_base
{
    using normaliztion_base::normaliztion_base;
};
std::string to_string(const z_normaliztion_conf& n);

std::ostream& operator << (std::ostream& os, const z_normaliztion_conf& nb);

struct max_normalization : normaliztion_base 
{
    using normaliztion_base::normaliztion_base;
};
std::string to_string(const max_normalization& n);
std::ostream& operator << (std::ostream& os, const max_normalization& nb);

struct minmax_normalization : normaliztion_base
{
    using normaliztion_base::normaliztion_base;

    double min = 0.0;
    double max = 0.0;

    minmax_normalization() = default;
    minmax_normalization(column_id id, 
            double a, double b = 0.0);

};
std::string to_string(const minmax_normalization& n);
std::ostream& operator << (std::ostream& os, const minmax_normalization& mmn);

