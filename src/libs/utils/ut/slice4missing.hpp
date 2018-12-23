#pragma once
#include "utils/math_utils.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include <boost/range/algorithm/copy.hpp>
#include <boost/iterator/function_input_iterator.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <vector>
#include <iostream>

using missing_input = std::vector<double>;
using data_set = missing_input;

template<typename It> inline
missing_input slice_data(const data_set& from, It pivot, std::size_t step)
{
    if (step == 1 && *pivot != 0u && *pivot < from.size()-2u) {
        auto sb = step;
        if (*(pivot-1) + 1 == *pivot) {
            ++sb;
        }
        auto sf = step;
        if (*(pivot+1) - 1 == *pivot) {
            ++sf;
        }
        return missing_input{from[*pivot - sb], from[*pivot + sf]};
    }
    auto st  = std::min(step, *pivot);
    auto ws = std::min(step, from.size()-*pivot) * 2 + 1;
    auto at = std::next(std::begin(from), (*pivot - st));
    auto l = std::next(at, ws);
    return missing_input(at, l);
}

template<typename It> inline
math::fx_list_type xi_values(It pivot, std::size_t step)
{
    if (step == 1 && *pivot != 0u) {
        auto sb = step;
        auto sf = step;
        if (*(pivot-1) + 1 == *pivot) {
            ++sb;
        }
        if (*(pivot+1) - 1 == *pivot) {
            ++sf;
        }
        math::fx_list_type::value_type p = static_cast<math::fx_list_type::value_type>(*pivot);
        return math::fx_list_type{p-sb, p+sf};
    } else {
        return math::fx_list_type{};
    }
}

struct reverse_generator
{
    using result_type = input_data;

    reverse_generator(const double* s, const double* t) :
                start(s), end(t), index(0)
    {
    }

    result_type operator () ()
    {
        if (start != end) {
            return result_type{*start++, index++};
        } else {
            return result_type{0, 0};
        }
    }

    const double* start = nullptr;
    const double* end = nullptr;
    unsigned int index = 0;
};


inline source_data 
from_double_array(const double* from, const double* to, 
        const std::string& name)
{
    reverse_generator gen{from, to};
    return source_data{name,
                    boost::make_function_input_iterator(gen, 0u),
                    boost::make_function_input_iterator(gen, (unsigned int)(to-from))
            };

}

template<typename It> inline source_data
filter_out(source_data input, It invaid_start, It invalid_end)
{
    auto i = boost::remove_if(input, [invaid_start, invalid_end](auto entry) {
                                return std::binary_search(invaid_start, invalid_end, entry.row);
                        }
            );
    return source_data{input.id(), std::begin(input), i};
}
