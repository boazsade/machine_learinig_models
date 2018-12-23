#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/fillinvalid_conf.h"
#include <iostream>


std::ostream& operator << (std::ostream& os, const kneighborhood_fix& kf)
{
    return os<<"kneighborhood, "<<static_cast<const fillinvalid_base&>(kf)<<",  count "<<kf.n_count;
}

std::ostream& operator << (std::ostream& os, const cubical_fix& kf)
{
    return os<<"cubical "<<static_cast<const fillinvalid_base&>(kf);
}

std::ostream& operator << (std::ostream& os, const linear_fix& kf)
{
    return os<<"linear "<<static_cast<const fillinvalid_base&>(kf);
}

kneighborhood_fix::kneighborhood_fix(const column_id& id, int count) :
    fillinvalid_base(id), n_count(count)
{
}

std::string to_string(const kneighborhood_fix&)
{
    return "kneighborhood_fix";
}

std::string to_string(const linear_fix&)
{
    return "linear_fix";
}

std::string to_string(const cubical_fix&)
{
    return "cubical_fix";
}


