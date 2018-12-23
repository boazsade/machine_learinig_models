#pragma once
#include <boost/multi_array.hpp>
#include <vector>

namespace mlmodels
{
    using value_type = float;
    using class_type = int;
    using classes_data = std::vector<class_type>;
    using class_data = std::vector<value_type>;
    using training_data = boost::multi_array<value_type, 2>;    // 2 dim array
    using models_input = training_data;
    using testing_data = training_data;
    using testing_result = class_data;

    bool empty(const training_data& td);
    std::size_t rows(const training_data& td);
    std::size_t columns(const training_data& td);
}   // end of namespace mlmodels

