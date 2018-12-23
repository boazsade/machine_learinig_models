#include "libs/sources_handling/source_data.h"
#include "utils/algorithms.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <assert.h>

source_data::source_data(const id_type& id) :
    the_id(id)
{
}

source_data::iterator source_data::begin()
{
    return all_data.begin();
}

source_data::iterator source_data::end()
{
    return all_data.end();
}

source_data::const_iterator source_data::begin() const
{
    return all_data.begin();
}

source_data::const_iterator source_data::end() const
{
    return all_data.end();
}

const source_data::value_type& source_data::row_data() const
{
    return all_data;
}


std::size_t size(const source_data& sd)
{
    return sd.row_data().size();
}

bool empty(const source_data& sd)
{
    return sd.row_data().empty();
}

source_data read(std::istream& from, const std::string& id)
{
    return source_data{id,
        std::istream_iterator<source_data::data_type>(from),
        std::istream_iterator<source_data::data_type>()
    };
}

std::istream& operator >> (std::istream& in, source_data& to)
{
    to = read(in, std::string{});
    return in;
}

const source_data::id_type& source_data::id() const
{
    return the_id;
}

std::insert_iterator<source_data::value_type>
source_data::make_inseter(source_data& with, std::size_t expeced_count)
{
    with.all_data.reserve(expeced_count);
    return std::inserter(with.all_data, with.begin());
}

std::insert_iterator<source_data::value_type>
make_inseter(std::size_t expected_elems, source_data& with)
{
    return source_data::make_inseter(with, expected_elems);
}

std::pair<source_data::iterator, source_data::iterator>
slice_it(source_data& from,  
        unsigned int pivot, unsigned int win_size)
{
    auto value = source_data::data_type{0, pivot};
    auto i = binary_find(from.begin(), from.end(), value, [](auto& entry, auto val) {
                return entry.row < val.row ;
            }
        );

    if (i != from.end()) {
        auto b = std::min(win_size, (unsigned int)std::distance(from.begin(), i)) ;
        auto e = std::min(win_size+1, (unsigned int)std::distance(i, from.end()));
        return std::make_pair(i-b, i+e);
    } else {
        return std::make_pair(i, i);
    }
}

source_data::algo_input 
transform_slice(
        std::pair<source_data::iterator, source_data::iterator> with)
{
    return transform_slice(with.first, with.second);
}

source_data::algo_input 
transform_slice(
        source_data::const_iterator from, source_data::const_iterator to)
{
    source_data::algo_input ret;
    std::transform(from, to, std::back_inserter(ret), [](auto& e) {
                    return e.value;
                }
            );
    return ret;
}

void merge_slice(const source_data::algo_input& slice,
        std::pair<source_data::iterator, source_data::iterator> at)
{
    assert((unsigned int)std::distance(at.first, at.second) >= slice.size());
    std::transform(std::begin(slice), std::end(slice), at.first, at.first,
            [](auto val, auto target) {
                return input_data{val, target.row};
            }
        );
}

source_data::algo_input
row_nums_transform(std::pair<source_data::iterator, source_data::iterator> range)
{
    return row_nums_transform(range.first, range.second);
}

source_data::algo_input
row_nums_transform(source_data::const_iterator from, source_data::const_iterator to)
{
    source_data::algo_input ret;
    std::transform(from, to, std::back_inserter(ret), [](auto& e) {
                    return static_cast<source_data::algo_input::value_type>(e.row);
                }
            );
    return ret;
}

source_data::iterator begin(source_data& sd)
{
    return sd.begin();
}

source_data::iterator end(source_data& sd)
{
    return sd.end();
}

source_data::const_iterator begin(const source_data& sd)
{
    return sd.begin();
}

source_data::const_iterator end(const source_data& sd)
{
    return sd.end();
}

source_data::iterator find(source_data& in, 
        source_data::data_type::index_type index)
{
    return std::find_if(begin(in), end(in), [index](auto& entry) {
                return entry.row == index;
            }
    );
}

source_data::const_iterator find(const source_data& in, 
        source_data::data_type::index_type index)
{
    return std::find_if(begin(in), end(in), [index](auto& entry) {
                return entry.row == index;
            }
    );
}


source_data::algo_input 
transform_slice(
        std::pair<source_data::iterator, source_data::iterator> with,
        source_data::data_type::value_type ignore_val)
{
    source_data::algo_input result = transform_slice(with);
    auto i = std::find(std::begin(result), std::end(result), ignore_val);
    result.erase(i);
    return result;
}

