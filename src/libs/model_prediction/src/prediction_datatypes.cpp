#include "libs/model_prediction/prediction_datatypes.h"

namespace prediction
{
std::ostream& operator << (std::ostream& os, column_number r)
{
    return os<<"column "<<r.value;
}

std::ostream& operator << (std::ostream& os, row_number r)
{
    return os<<"row "<<r.value;
}

}   // end of namespace prediction
