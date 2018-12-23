#include "utils/dc_lookup_table.h"
#include "utils/algorithms.hpp"
#include <boost/range/algorithm/sort.hpp>
#include <iostream>

namespace lookup
{

std::ostream& operator << (std::ostream& os, const table_entry& te)
{
    return os<<"\""<<te.source<<"\": "<<te.target;
}

///////////////////////////////////////////////////////////////////////////////

void table::sort()
{
    boost::sort(entries);
}

void table::insert(table_entry::source_type val)
{
    auto i = find(val);
    if (i == std::end(entries)) {   // new value
        entries.emplace_back(val, current++);
        sort();
    }
}

table::const_iterator table::find(table_entry::source_type val) const
{
    return binary_find(std::begin(entries), std::end(entries), val, [](auto left, auto right) { return left < right; });
}

bool table::empty() const
{
    return entries.empty();
}

std::size_t table::size() const
{
    return entries.size();
}

table_entry::result_type
    table::operator [] (table_entry::source_type index) const
{
    auto i = find(index);
    if (i == std::end(entries)) {
        return invalid_entry();
    } else {
        return i->target;
    }
}

const table::list_type& table::data() const
{
    return entries;
}

bool empty(const table& t)
{
    return t.empty();
}

std::size_t size(const table& t)
{
    return t.size();
}

table_entry::result_type 
    mapped(const table& t, table_entry::source_type index)
{
    return t[index];
}

void insert(table& t, table_entry::source_type index)
{
    t.insert(index);
}

std::ostream& operator << (std::ostream& os, const table& tbl)
{
    int c = 0;
    os<<"{";
    for (const auto& item : tbl.data()) {
        if (c) {
            os<<", ";
        }
        os<<item;
        ++c;
    }
    os<<"}";
    return os;
    
}

}   // end of namespace lookup
