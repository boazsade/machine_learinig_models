#pragma once
#include "model_types.h"
#include <string>
#include <iostream>

namespace actions
{

inline std::string to_string(model_type mt) 
{
    using namespace std::string_literals;
    switch (mt) {
        case model_type::CLASSIFIER_MODEL:
            return "classifier"s;
        case model_type::REGRESSION_MODEL:
            return "regression"s;
        default:
            return "unknown"s;
    }
}

inline std::ostream& operator << (std::ostream& os, model_type mt)
{
    return os<<to_string(mt);
}

}   // end of namespace actions

