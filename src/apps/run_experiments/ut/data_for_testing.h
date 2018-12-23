#pragma once
#include "columns4testing.h"
#include "libs/sources_handling/row_data_input.h"
#include "utils/algorithms.hpp"
#include <string>

namespace flowut
{

const std::string column_idx[] = {
    "one", "two", "three", "four", "five"
};

const auto classes_column_id = column_idx[4];
const unsigned int split_size = 80;

inline
classifier::read_result::value_type 
    row2input(std::size_t index) {
    
        static const std::vector<const double*> all_row_data {
            data_for_1, data_for_2, data_for_3, data_for_4, data_for_5
        };
        static const auto data_size = utils::size(data_for_1);

        return classifier::read_result::value_type{column_idx[index], 
                                all_row_data[index],
                                all_row_data[index] + data_size
        };
}

inline
const classifier::read_result& data4test()
{
    static const classifier::read_result souce_data{
        row2input(0), row2input(1), row2input(2), 
        row2input(3), row2input(4)
    };
    return souce_data;
}

}   // end of namespace flowut
