#pragma once
#include <boost/range/algorithm/sort.hpp>
#include <string>
#include <vector>
#include <iosfwd>

struct filterout_conf
{
    using column_id = std::string;
    using row_numbers = std::vector<std::size_t>;

    filterout_conf() = default;

    explicit filterout_conf(const column_id& id);

    template<typename It>
    filterout_conf(const column_id& id, 
            It from, It to) :
        column(id), to_remove(from, to)
    {
        boost::range::sort(to_remove);
    }

    column_id   column;
    row_numbers to_remove;
};

bool valid(const filterout_conf& fc);

std::ostream& operator << (std::ostream& os, const filterout_conf& fc);

bool operator == (const filterout_conf& left, const filterout_conf& right);
bool operator != (const filterout_conf& left, const filterout_conf& right);
bool operator < (const filterout_conf& left, const filterout_conf& right);
bool operator == (const filterout_conf::column_id& left, const filterout_conf& right);
bool operator != (const filterout_conf::column_id& left, const filterout_conf& right);
bool operator < (const filterout_conf::column_id& left, const filterout_conf& right);
bool operator == (const filterout_conf& left, const filterout_conf::column_id& right);
bool operator != (const filterout_conf& left, const filterout_conf::column_id& right);
bool operator < (const filterout_conf& left, const filterout_conf::column_id& right);
std::string to_string(const filterout_conf&);

