#pragma once
#include "prediction_status.h"
#include "prediction_datatypes.h"
#include "libs/json_parser/json_ostream.h"
#include <boost/variant.hpp>
#include <limits>
#include <iosfwd>
#include <string>

namespace prediction
{

struct output
{
private:
public:
    
    output() = default;

    explicit output(row_number n);

    explicit output(data_type v);
    output(row_number n, data_type v);
    explicit output(status s);
    output(row_number n, status s);

    output& operator = (row_number n);
    output& operator = (data_type v);
    output& operator = (status s);
    friend std::string to_string(const output& o);
    friend json::ostream& to_json(json::ostream& jo, const output& o);

    friend constexpr row_number index(const output& o) {
        return o.row_num;
    }

    friend status state(const output& o);
    friend data_type get_value(const output& o);
private:
    using storage_t = boost::variant<status, data_type>;
    storage_t data;
    row_number row_num;
};
std::ostream& operator << (std::ostream& os, const output& o);
bool valid_output(const output& o);

}   // end of namespace prediction

