#pragma once
#include "column_stats.h"
#include <string>
#include <iosfwd>


struct duplication_conf {
    using resolution = int;

    enum how {
        KEEP_FIRST = 0,
        KEEP_LAST,
        NO_DEFINED
    };
    
    column_id col_id;
    resolution res = 1;
    how      act = NO_DEFINED;

    duplication_conf() = default;
    duplication_conf(const column_id& id, resolution re, const std::string& ac);
    duplication_conf(const column_id& id, resolution re, how h);

    static  how convert2act(const std::string& str);
};

bool operator == (const duplication_conf& left, const duplication_conf& right);
bool operator != (const duplication_conf& left, const duplication_conf& right);
bool operator < (const duplication_conf& left, const duplication_conf& right);

std::ostream& operator << (std::ostream& os, const duplication_conf& dc);

