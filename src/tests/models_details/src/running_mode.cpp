#include "apps/run_experiments/running_mode.h"
#include <iostream>

//namespace classifier
//{

percent_split::percent_split(unsigned int s,
            const std::string& tid) : split(s), target_id(tid)
{
}

k_fold::k_fold(unsigned int s, 
        const std::string& tid) : split(s), target_id(tid)
{
}

std::ostream& operator << (std::ostream& os, const k_fold& kf)
{
    return os<<"k fold running mode: "<<kf.split<<", target id '"<<kf.target_id<<"'";
}

std::ostream& operator << (std::ostream& os, const percent_split& ps)
{
    return os<<"precent split mode: "<<ps.split<<", target id '"<<ps.target_id<<"'";
}

//}  // end of namespace classifier

