#include "libs/model_prediction/prediction_row.h"
#include "libs/sources_handling/schema_read.hpp"
#include "libs/system/exceptions.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>
#include "logging.h"

namespace prediction
{

namespace 
{

schema_type read_row(std::istream& from)
{
    return sr::read_schema(from, " \t", [](auto token) { return token; });
}

row parse_row(schema_type&& raw_entries, const schema_mapper& scheme)
{
    static const std::string row_number_name = "index";
    const auto expected_cols = scheme.columns_num() - 1u; 
    // we need to map the values to the locations we have and return them, 
    row::value_type d(expected_cols, row::data_type{});
    row::index_type row_n;
    const auto end_it = make_schemaid_end_iterator();
    for (auto ni = make_schemaid_iterator(scheme); ni != end_it; ++ni) {
        auto index = scheme[*ni];
        //std::cout<<"handling column name '"<<*ni<<"' with index "<<index<<" for entry "<<raw_entries[index]<<"\n";
        if (*ni == row_number_name) {   // we must ignore this entry this would be saved for row number
            //std::cout<<"saving column number "<<raw_entries[index]<<std::endl;
            row_n= std::atoi(raw_entries[index].c_str());
        } else {
            d[index] = std::atof(raw_entries[index].c_str());
        }
    }
    //std::cout<<"we have "<<d.size()<<" entries and row "<<row_n<<" number\n";
    return row{std::begin(d), std::end(d), row_n};
}

}   // end of namespace

row::row(index_type i) :
    number{i}
{
}

bool row::empty() const
{
    return data.empty();
}

std::size_t row::size() const
{
    return data.size();
}

const row::data_type* row::row_data() const
{
    return data.data();
}

row::iterator row::begin()
{
    return data.begin();
}

row::iterator row::end()
{
    return data.end();
}

row::const_iterator row::begin() const
{
    return  data.begin();
}

row::const_iterator row::end() const
{
    return data.end();
}

row::data_type row::operator [] (std::size_t at) const
{
    return data[at];
}

row::data_type& row::operator [] (std::size_t at)
{
    return data[at];
}

std::ostream& operator << (std::ostream& os, const row& r)
{
    return os<<to_string(r);
}

std::string to_string(const row& r)
{
    return "row number " + to_string(row_num(r)) +
        ", columns: " + std::to_string(r.size());
}

row read(std::istream& from, const schema_mapper& scheme)
{
    if (from && not scheme.empty()) {
        auto d{read_row(from)};
        LOG_DEBUG<<"read "<<d.size()<<" columns for the row";
        if (d.size() == scheme.columns_num()) {
            return parse_row(std::move(d), scheme);
        } else {
            LOG_WARNING<<"invalid row read from input (size "<<d.size()<<" != "<<scheme.columns_num()<<")!!";
            if (from.eof()) {
                LOG_WARNING<<"no more data in the file";
            } else {
                LOG_ERROR<<"invalid row was read as a result on invalid input stream";
                throw invalid_input{"cannot process source file"};
            }
        }
    } else {
        LOG_WARNING<<"we have "<<(from ? "empty schema!!" : "invalid stream!")<<", cannot read row";
    }
    return row{};   // error case
}

row::row(index_type rn, unsigned int columns) :
    number{rn}, data(columns, data_type{})
{
}

}   // end of namespace prediction

