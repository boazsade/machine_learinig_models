#pragma once
#include "libs/classfication_algos/general_types.h"
#include "schema_type.h"
#include <iosfwd>
#include <vector>
// the data type that is being use for reading the input and building a list 
// of columns from it
struct column_data
{
    using id_type = schema_type::value_type;
    using data_type = std::vector<classifier::value_type>;

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

read_result read(std::istream& from, const schema_type& schema);

training_data data_matrix(const read_result& row_data, 
        const column_data::id_type& label_col, unsigned int rows);

class_data labels_data(const read_result& row_data,
        const column_data::id_type& label_col, unsigned int rows);



}   // end of namespace classifier
