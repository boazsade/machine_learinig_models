#pragma once
#include "column_stats.h"
#include <iosfwd>

struct outliers_conf
{
    column_id col_id;

    outliers_conf() = default;
    explicit outliers_conf(const column_id& id);
};

bool operator == (const outliers_conf& left, const outliers_conf& right);
bool operator != (const outliers_conf& left, const outliers_conf& right);
bool operator < (const outliers_conf& left, const outliers_conf& right);

std::ostream& operator << (std::ostream& os, const outliers_conf& oc);
