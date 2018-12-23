#include "libs/model_prediction/prediction_output.h"
#include "utils/missing_values.hpp"
#include <iostream>
#include <cmath>

namespace prediction
{

namespace
{

struct as_string : boost::static_visitor<std::string>
{
    std::string operator () (data_type v) const {
        return std::to_string(v);
    }

    std::string operator () (status s) const {
        return to_string(s);
    }

};

struct as_json :
    boost::static_visitor<> {

    as_json(json::ostream& j) :
        js{j}
    {
    }

    void operator () (data_type v) {
        using namespace json::literals;
        js ^ "value"_n ^ v;
    }

    void operator () (status s) {
        using namespace json::literals;
        js ^ "status"_n ^ to_string(s);
    }

    json::ostream& js;
};

struct extract_status :
    boost::static_visitor<status> 
{
    status operator () (status s) const
    {
        return s;
    }

    status operator () (data_type v) const
    {
        return missing_value(v) ? status::INVALID : status::OK;
    }
};

struct extract_value :
    boost::static_visitor<data_type>
{
    data_type operator () (status) const
    {
        return missing_value<data_type>();
    }

    data_type operator () (data_type d) const
    {
        return d;
    }
};

}   // end of namespace

output::output(prediction::data_type v) :
    data{v}
{
}

output::output(status s) : 
    data{s}
{
}

output::output(row_number n, prediction::data_type v) : 
    data{v}, row_num{n}
{
}

output::output(row_number n, status s) :
    data{s}, row_num{n}
{
}

output& output::operator=(row_number n) 
{
    row_num = n;
    return *this;
}

output& output::operator = (prediction::data_type v)
{
    data = v;
    return *this;
}

output& output::operator = (status s) 
{
    data = s;
    return *this;
}

std::ostream& operator << (std::ostream& os, const output& o)
{
    return os<<to_string(o);
}

std::string to_string(const output& o)
{
    using namespace std::string_literals;

    return "row: "s + to_string(o.row_num) + " "s +  boost::apply_visitor(as_string{}, o.data);
}

json::ostream& to_json(json::ostream& jo, const output& o)
{
    using namespace json::literals;
    if (not valid_output(o)) {
        return jo;
    }
    jo ^ "row"_n ^ o.row_num.value;
    as_json parser{jo}; 
    boost::apply_visitor(parser, o.data);
    return jo;
}

status state(const output& o)
{
    return boost::apply_visitor(extract_status{}, o.data);
}

data_type get_value(const output& o)
{
    return boost::apply_visitor(extract_value{}, o.data);
}

bool valid_output(const output& o)
{
    return not invalid_row(index(o));   // this is a sign that we don't really have someting here
}

}   // end of namespace prediction
