#include "libs/model_prediction/prediction_data.h"
#include <iostream>

namespace prediction
{

namespace
{

struct validate : 
    boost::static_visitor<bool>
{
    bool operator () (const output&) const
    {
        return false;
    }

    bool operator () (const row& r) const
    {
        return not r.empty();
    }
};

struct get_size : 
    boost::static_visitor<std::size_t>
{
    std::size_t operator () (const output&) const
    {
        return 0u;
    }

    std::size_t operator () (const row& r) const
    {
        return r.size();
    }
};

struct extract_output :
    boost::static_visitor<output> 
{
    const output& operator ()(const output& o) const
    {
        return o;
    }

    output operator () (const row& r) const
    {
        return output{index(r), status::OK};
    }
};

struct read_data :
    boost::static_visitor<const row&>
{
    const row& operator () (const output& ) const
    {
        static const row invalid_data{invalid_row()};
        return invalid_data;
    }

    const row& operator () (const row& r) const
    {
        return r;
    }
};

}   // end of namespace

data::data(row r) : value{std::move(r)}
{
}

data::data(output o) : value{std::move(o)}
{
}

data::operator data::boolean_type () const
{
    return boost::apply_visitor(validate{}, value) ? &data::dummy : (boolean_type)nullptr;
}


bool empty(const data& d)
{
    return boost::apply_visitor(get_size{}, d.value) == 0u;
}

std::size_t size(const data& d)
{
    return boost::apply_visitor(get_size{}, d.value);
}
bool good(const data& d)
{
    return d ? true : false;
}

bool bad(const data& d)
{
    return !(good(d));
}

std::ostream& operator <<(std::ostream& os, const data& d)
{
    return os<<d.value;
}

output current_state(const data& d)
{
    return boost::apply_visitor(extract_output{}, d.value);
}

std::string to_string(column_number r)
{
    return std::to_string(r.value);
}

std::string to_string(row_number r)
{
    return std::to_string(r.value);
}

const row& current_values(const data& d) 
{
    return boost::apply_visitor(read_data{}, d.value);
}

}   // end of namespace prediction

