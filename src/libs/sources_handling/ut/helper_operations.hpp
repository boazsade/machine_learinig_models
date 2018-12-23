#pragma once
#include "utils/math_utils.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include <boost/range/algorithm/copy.hpp>
#include <boost/iterator/function_input_iterator.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <vector>
#include <iostream>

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

