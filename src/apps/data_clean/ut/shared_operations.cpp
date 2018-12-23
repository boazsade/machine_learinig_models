#include "shared_operations.h"
#include <boost/iterator/function_input_iterator.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>

bool operator == (const source_data& left, const source_data& right)
{
    return left.id() == right.id() && 
        size(left) == size(right) && 
        std::equal(left.begin(), left.end(),
                right.begin(), right.end());
}

bool operator != (const source_data& left, const source_data& right)
{
    return !(left == right);
}

bool operator == (const input_data& left, const input_data& right)
{
    return left.row == right.row && left.value == right.value;
}

bool operator != (const input_data& left, const input_data& right)
{
    return !(left == right);
}

struct generator
{
    using result_type = input_data;

    generator() = default;

    result_type operator () () const
    { 
        if (index > 0 &&  index % 7  == 0)  { 
            ++index; 
        } 
        ++index; 
        return input_data{index*20/7.56+3.1, index};
    }

    mutable unsigned int index = 0u;
};

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

source_data make_test_data(unsigned int size, const source_data::id_type& id)
{
    generator f;

    source_data sd(id, boost::make_function_input_iterator(f, 0u),
                   boost::make_function_input_iterator(f, size));
    
    return sd;
}

std::ostream& operator << (std::ostream& os, const source_data& sd)
{
    os<<"id: '"<<sd.id()<<"', data size "<<size(sd)<<"\ndata: [";
    std::copy(sd.begin(), sd.end(), std::ostream_iterator<source_data::data_type>(os, ", "));
    return os<<"]";
}

source_data from_double_array(const double* from, const double* to, const std::string& name)
{
    reverse_generator gen{from, to};
    return source_data{name,
                boost::make_function_input_iterator(gen, 0u),
                boost::make_function_input_iterator(gen, (unsigned int)(to-from))
    };
}

std::ostream& operator << (std::ostream& os, source_data::iterator )
{
    return os<<"iterator";
}

