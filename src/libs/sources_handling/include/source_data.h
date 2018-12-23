#pragma once
#include "libs/sources_handling/input_data.h"
#include "utils/algorithms.hpp"
#include <vector>
#include <iosfwd>
#include <string>
#include <iterator>
#include <algorithm>

struct source_data
{

    using data_type = input_data;
    using algo_data = double;
    using value_type = std::vector<data_type>;
    using algo_input = std::vector<algo_data>;
    using id_type = std::string;
    using iterator = value_type::iterator;
    using const_iterator = value_type::const_iterator;

    source_data() = default;
    explicit source_data(const id_type& i);

    template<typename It>
    source_data(const id_type& i, It from, It to) :
        the_id(i), all_data{from, to}
    {
    }

    source_data(source_data&&) = default;
    source_data& operator = (source_data&&) = default;
    source_data(const source_data&) = default;
    source_data& operator = (const source_data&) = default;

    source_data& operator = (const id_type& id);

    const value_type& row_data() const;
    const id_type& id() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    static std::insert_iterator<source_data::value_type>
    make_inseter(source_data& with, std::size_t expeced_count);

private:
    id_type    the_id;
    value_type all_data;
};

source_data read(std::istream& from, const std::string& id);
std::istream& operator >> (std::istream& in, source_data& to);
template<typename Algo> inline
source_data transform(const source_data& data, Algo operation) {
    return operation(data.row_data());
}


source_data::iterator begin(source_data& sd);
source_data::iterator end(source_data& sd);
source_data::const_iterator begin(const source_data& sd);
source_data::const_iterator end(const source_data& sd);
source_data::iterator find(source_data& in, 
        source_data::data_type::index_type index);
source_data::const_iterator find(const source_data& in, 
        source_data::data_type::index_type index);

std::size_t size(const  source_data& );
bool empty(const source_data& );

std::insert_iterator<source_data::value_type>
make_inseter(std::size_t expected_elems, source_data& with);

// return sub range of the values that we need to pass out to
// fixing algorithm
std::pair<source_data::iterator, source_data::iterator>
slice_it(source_data& from, unsigned int pivot, unsigned int win_size);

namespace details
{

template<typename Iter>
inline source_data::value_type
slice_some(source_data::const_iterator start, source_data::const_iterator end,
        Iter pivot, int step, std::size_t count)
{
    source_data::value_type res; 
    if (end != start && *pivot != 0u) {
        start = std::next(start, step);
        pivot = std::next(pivot, step);
        while (count > 0u) {
            if (start->row == *pivot) {
                pivot = std::next(pivot, step);
            } else {
                res.push_back(*start);
                --count;
            }
            start = std::next(start, step);
        }
    }
    return res;
}

}

// this is different in the sence that we are passing the location
// in the list around which we need to find which values are valid
// note that in this case we are checking if the value that we have
// in the range we are returning would all have enough valid values
// as well - that is the difference beween the returned iterator
// may be larger than win_size
template<typename Iterator>
inline source_data::value_type
slice(const source_data& from, Iterator invalid_point, unsigned int win_size)
{
    source_data::data_type v{0, *invalid_point};
    auto i = binary_find(from.begin(), from.end(), v, [] (auto& entry, auto val) {
                return entry.row < val.row;
            }
    );
    if (i == from.end()) {
        return source_data::value_type{};
    }
    auto tmp{details::slice_some(i, begin(from), invalid_point, -1, win_size)};
    source_data::value_type result(tmp.rbegin(), tmp.rend());
    tmp = details::slice_some(i, std::end(from), invalid_point, 1, win_size);
    result.insert(result.end(), std::begin(tmp), std::end(tmp));
    return result;
}

// return a slice from the source data with pivot as the index
// in the data (not the offset but the actual index)
// and win_size is the amount of data around each if 
// size of the pivot
source_data::algo_input 
transform_slice(
        std::pair<source_data::iterator, source_data::iterator> with);

source_data::algo_input 
transform_slice(
        source_data::const_iterator from, source_data::const_iterator to);

// as above but return a sub set of the data that we are calling
// while ignoring the ignore_val
source_data::algo_input 
transform_slice(
        std::pair<source_data::iterator, source_data::iterator> with,
        source_data::data_type::value_type ignore_val);
// this would merge a slice of data into the source_data
// using the index and the "real" index to which to merge
// that is this is not the offset but the actual index
// in the source data
void merge_slice(const source_data::algo_input& slice,
        std::pair<source_data::iterator, source_data::iterator> at);


// extract the row numbers from the range - do this while convering
// them into doubles as this is the type that we are passing to algorithms
source_data::algo_input
row_nums_transform(std::pair<source_data::iterator, source_data::iterator> range);

source_data::algo_input
row_nums_transform(source_data::const_iterator from, source_data::const_iterator to);

// this function can be used to filter out rows that we do not
// want to leave in the list of rows - this function assume that 
// both lists are sorted!!
// note that we do not actualy remove the elements, but it is possible to do so
template<typename Iter> inline
source_data::iterator filter_out(source_data::iterator from, source_data::iterator to,
                Iter range_from, Iter range_to)
{
    return std::remove_if(from, to, [range_from, range_to](const auto& entry) {
                        return std::binary_search(range_from, range_to, entry.row);
                    }
            );
}

