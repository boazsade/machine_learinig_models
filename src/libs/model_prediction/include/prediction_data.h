#pragma once
#include "prediction_row.h"
#include "prediction_output.h"
#include <boost/variant.hpp>
#include <iosfwd>

namespace prediction
{
    
struct data
{
private:
    void dummy() const {}
public:
    using boolean_type = void(data::*)()const;

    using value_t = 
        boost::variant<output, row>;

    explicit data(row r);
    explicit data(output o);
    
    operator boolean_type () const;

    value_t value;
};

bool empty(const data& d);
std::size_t size(const data& d);
std::ostream& operator <<(std::ostream& os, const data& d);
bool good(const data& d);
bool bad(const data& d);
output current_state(const data&);  // return the output object which is the status right now
const row& current_values(const data& d);  // if state != OK, would return empty row

}   // end of namespace prediction

