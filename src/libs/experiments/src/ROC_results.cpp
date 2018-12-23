#include "libs/experiments/ROC_results.h"
#include <iostream>
#include <iterator>
#include <boost/range/algorithm/copy.hpp>

namespace exprs
{

std::ostream& operator << (std::ostream& os, const ROC_results& roc)
{
    os<<"area: "<<roc.area<<", accuracy: "<<roc.accuracy<<", RMSE: "<<roc.rmse<<"\nsensivity: ";
    boost::copy(roc.sensitivity, std::ostream_iterator<data_array::value_type>(os, ", "));
    os<<"\nfall out: ";
    boost::copy(roc.fall_out, std::ostream_iterator<data_array::value_type>(os, ", "));
    return os;
}

}   // end of namespace exprs

