#include "testing_input_task.h"
#include "row_data_input.h"
#include "logging.h"

namespace classifier
{

actions::testing_input preform_action(const test_run<percent_split>& conf, 
                                    const classifier::read_result& data)
{
    LOG_DEBUG<<"running testing with precent split";
    auto s = data.size() * 100u;    // we would need to turn this to percent
    auto cut = conf.split;          // for test we are taking the lower part of 100%
    s /= cut;
    LOG_INFO<<"taking "<<s<<" rows for testing out of "<<data.size();
    return data_matrix(data, conf.target_id, s);
}

actions::testing_input preform_action(const test_run<k_fold>& , 
                                    const classifier::read_result& )
{
    LOG_DEBUG<<"running testing with k-fold";
    LOG_ERROR<<"this is not implemented yet";
    return actions::testing_input{};
}

}   // end of namespace classifier

