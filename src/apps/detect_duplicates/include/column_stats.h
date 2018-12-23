#pragma once

#include <string>
#include <vector>
#include <iosfwd>

using column_id = std::string;
using row_numbers = std::vector<unsigned int>; // we are saving the row numbers here

struct column_stats
{
    column_id   column;
    std::string path;
    std::size_t total = 0;
    row_numbers missings;

    column_stats() = default;
    column_stats(const column_id& id, const std::string& p);
};

std::ostream& operator << (std::ostream& os, const column_stats& stats);

bool operator < (const column_stats& left, const column_stats& right);
bool operator == (const column_stats& left, const column_stats& right);
bool operator != (const column_stats& left, const column_stats& right);

