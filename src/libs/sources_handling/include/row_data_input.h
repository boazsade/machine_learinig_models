#pragma once
#include "libs/ml_models/general_types.h"
#include "schema_type.h"
#include <algorithm>
#include <iosfwd>
#include <vector>
// the data type that is being use for reading the input and building a list 
// of columns from it
struct column_data
{
    using id_type = schema_type::value_type;
    using data_type = std::vector<mlmodels::value_type>;

    column_data() = default;
    column_data(const id_type& id);
    template<typename It>
    column_data(const id_type& id, It from, It to) :
        ID(id), data(from, to)
    {
    }

    id_type ID;
    data_type data;
};

std::ostream& operator << (std::ostream& os, 
        const column_data& cd);

schema_type read_schema(std::istream& from);

namespace classifier
{

using read_result = std::vector<column_data>;

bool empty(const read_result& rr);
std::size_t rows(const read_result& rr);
std::size_t columns(const read_result& rr);
const read_result::value_type::data_type& 
    data_entry(const read_result& from, const column_data::id_type& id);

read_result read(std::istream& from, const schema_type& schema);
read_result&& randomalized(read_result from);

mlmodels::training_data data_matrix(const read_result& row_data, 
        const column_data::id_type& label_col, unsigned int rows,
        unsigned int off = 0);

mlmodels::class_data labels_data(const read_result& row_data,
        const column_data::id_type& label_col, 
        unsigned int rows, unsigned int off = 0);

mlmodels::classes_data transform2labels(const read_result& from,
                    const column_data::id_type& label_col,
                    unsigned int rows, unsigned int off = 0);

}   // end of namespace classifier
