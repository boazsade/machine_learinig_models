#include "libs/model_prediction/prediction_status.h"
#include <iostream>

namespace prediction
{

std::ostream& operator << (std::ostream& os, status s)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    const auto a{to_string(s)};
    return os<<a;
#pragma GCC diagnostic pop
}

std::string to_string(status s)
{
    using namespace std::string_literals;
    switch (s) {
        case status::OK:
        case status::END_OF_FILE:
            return "OK"s;
        case status::REMOVED:
            return "removed"s;
        case status::MODEL_ERROR:
            return "model-error"s;
        case status::INVALID:
            return "invalid-data"s;
    }
}

}   // end of namespace prediction

