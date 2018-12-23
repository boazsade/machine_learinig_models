#pragma once
#include <iosfwd>
#include <vector>
#include <limits>
#include <algorithm>
#include <iosfwd>

namespace lookup
{

struct table_entry
{
    using source_type = double;
    using result_type = int;

    constexpr table_entry() = default;
    constexpr table_entry(source_type s,
                result_type r) :
                    source{s}, target{r}
    {
    }

    static constexpr 
        result_type invalid_entry() {
            return std::numeric_limits<result_type>::max();
        }
    source_type source = 0;
    result_type target = invalid_entry();
};
std::ostream& operator << (std::ostream& os, const table_entry& te);
constexpr inline 
    bool operator == (const table_entry& left, const table_entry& right)
{
    return left.source == right.source &&
        left.target == right.target;
}

constexpr inline 
    bool operator != (const table_entry& left, const table_entry& right)
{
    return !(left == right);
}

constexpr inline 
    bool operator < (const table_entry& left, const table_entry& right)
{
    return left.source < right.source;
}

constexpr inline 
    bool operator < (table_entry::source_type left, const table_entry& right)
{
    return left < right.source;
}

constexpr inline 
    bool operator < (const table_entry& left, table_entry::source_type& right)
{
    return left.source < right;
}

struct table
{
    using list_type = std::vector<table_entry>;
    using value_type = list_type::value_type;

    table() = default;

    template<typename It>
    table(It from, It to) :
      entries(from, to)
    {
        sort();
    }

    static constexpr 
        table_entry::result_type invalid_entry()
    {
        return table_entry::invalid_entry();
    }

    void insert(table_entry::source_type val);

    table_entry::result_type    // if not found would return invalid_entry 
        operator [] (table_entry::source_type index) const;
     
    bool empty() const;
    std::size_t size() const;
    const list_type& data() const;
private:

    using const_iterator = list_type::const_iterator;

    void sort();
    const_iterator find(table_entry::source_type what) const;

    list_type  entries;
    table_entry::result_type current = 0;
};

bool empty(const table& t);
std::size_t size(const table& t);
table_entry::result_type 
    mapped(const table& t, table_entry::source_type index);
void insert(table& t, table_entry::source_type index);

template<typename It> inline
table map(It from, It to) {
    table lut;
    std::for_each(from, to, [&lut](auto val) {
            insert(lut, val);
        }
    );
    return lut;
}

std::ostream& operator << (std::ostream& os, const table&);
}   // end of namespace lookup
