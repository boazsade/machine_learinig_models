#pragma once
#include "prediction_datatypes.h"
#include "libs/sources_handling/map_schema.h"
#include <iosfwd>
#include <string>
#include <limits>

namespace prediction
{

struct row
{
    using value_type = row_data_t;
    using index_type = row_number;
    using iterator = value_type::iterator;
    using const_iterator = value_type::const_iterator;
    using data_type = value_type::value_type;

    row() = default;

    explicit row(index_type i);
    row(index_type i, unsigned int columns);

    template<typename It>
        row(It data_be, It data_ed, index_type i) :
                number{i},
                data(data_be, data_ed)
        {
        }

    friend constexpr index_type row_num(const row& r) {
        return r.number;
    }

    bool empty() const;

    std::size_t size() const;

    const data_type* row_data() const;

    friend constexpr index_type index(const row& r) {
        return r.number;
    }

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    data_type operator [] (std::size_t at) const;
    data_type& operator [] (std::size_t at);

private:
    index_type number = 
        std::numeric_limits<index_type>::max();
    value_type data;
};

inline bool good(const row& r) {
    return not r.empty();
}

inline bool bad(const row& r) {
    return !good(r);
}

inline bool columns(const row& r) {
    return r.size();
}

std::ostream& operator << (std::ostream& os, const row& r);
std::string to_string(const row& r);

row read(std::istream& from, const schema_mapper& scheme);

}   // end of namespace prediction

