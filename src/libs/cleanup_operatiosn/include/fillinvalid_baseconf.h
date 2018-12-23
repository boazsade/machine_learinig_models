#pragma once
#include <boost/range/algorithm/sort.hpp>
#include <iosfwd>
#include <vector>

struct basic_fillinvalid
{
    using column_id = int;
    using row_number = unsigned int;
    
    constexpr basic_fillinvalid() = default;
    constexpr explicit basic_fillinvalid(column_id id) :
            column{id}
    {
    }

    column_id column = -1;
};
std::string to_string(basic_fillinvalid bf);
std::ostream& operator << (std::ostream& os, const basic_fillinvalid& bf);

constexpr struct _default_fill {} default_fill = _default_fill{};
inline std::string to_string(_default_fill) { return "default fill"; };
std::ostream& operator << (std::ostream& os, _default_fill);


struct fillinvalid_base : basic_fillinvalid
{
    using row_numbers = std::vector<row_number>;

    fillinvalid_base() = default;

    explicit fillinvalid_base(column_id id);

    template<typename It>
    fillinvalid_base(column_id id, It from, It to) : 
        basic_fillinvalid{id}, invalid_rows(from, to)
    {
        boost::range::sort(invalid_rows);    
    }

    row_numbers invalid_rows;
};

std::ostream& operator << (std::ostream& os, const fillinvalid_base& fb);
std::string to_string(const fillinvalid_base&); 

bool operator == (const fillinvalid_base& left, const fillinvalid_base& right);
bool operator != (const fillinvalid_base& left, const fillinvalid_base& right);
bool operator < (const fillinvalid_base& left, const fillinvalid_base& right);
bool operator == (fillinvalid_base::column_id left, const fillinvalid_base& right);
bool operator != (fillinvalid_base::column_id left, const fillinvalid_base& right);
bool operator < (fillinvalid_base::column_id left, const fillinvalid_base& right);
bool operator == (fillinvalid_base& left, fillinvalid_base::column_id right);
bool operator != (const fillinvalid_base& left, fillinvalid_base::column_id right);
bool operator < (const fillinvalid_base& left, fillinvalid_base::column_id right);

