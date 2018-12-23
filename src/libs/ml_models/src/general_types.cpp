#include "libs/ml_models/general_types.h"

namespace mlmodels
{
    

bool empty(const training_data& td)
{
    return td.empty();
}

std::size_t rows(const training_data& td)
{
    return empty(td) ? 0u : td.shape()[0];
}

std::size_t columns(const training_data& td)
{
    return empty(td) ? 0u : td.shape()[1];
}


}   // end of namespace mlmodels

