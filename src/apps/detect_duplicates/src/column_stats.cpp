#include "column_stats.h"
#include <iostream>
#include <iterator>
#include <algorithm>

column_stats::column_stats(const column_id& id, const std::string& p) :
    column(id), path(p)
{
}

std::ostream& operator << (std::ostream& os, const column_stats& stats)
{
    os<<"column id: ["<<stats.column<<"], location: ["<<stats.path
        <<"], total rows: "<<stats.total<<"\nmissing rows:\n";
    std::copy(std::begin(stats.missings), std::end(stats.missings),
            std::ostream_iterator<row_numbers::value_type>(os, ","));
    return os;
    
}

bool operator < (const column_stats& left, const column_stats& right)
{
    return left.column < right.column;
}

bool operator == (const column_stats& left, const column_stats& right)
{
    return left.column == right.column;
}

bool operator != (const column_stats& left, const column_stats& right)
{
    return !(left == right);
}

