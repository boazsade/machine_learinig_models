#include "libs/model_data/running_mode.h"
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

std::string to_string(const train_run<percent_split>& tr)
{
    return "train-percent[" + std::to_string(tr.split) + ":" + tr.target_id + "]";
}

std::string to_string(const test_run<percent_split>& tr)
{
    return "test-percent[" + std::to_string(tr.split) + ":" + tr.target_id + "]";
}

std::string to_string(const train_run<k_fold>& tr)
{
    return "train-percent[" + std::to_string(tr.split) + ":" + tr.target_id + "]";
}

std::string to_string(const test_run<k_fold>& tr)
{
    return "test-percent[" + std::to_string(tr.split) + ":" + tr.target_id + "]";
}



//}  // end of namespace classifier

