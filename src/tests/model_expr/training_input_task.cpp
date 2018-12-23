#include "training_input_task.h"
#include "logging.h"

namespace classifier
{

actions::training_input preform_action(const train_run<percent_split>& conf, 
                                    const classifier::read_result& data)
{
    LOG_DEBUG<<"processing data with precent split for training";
    auto s = data.size() * 100u;    // we would need to turn this to percent
    auto cut = (100u - conf.split);          // for test we are taking the lower part of 100%
    s /= cut;
    LOG_INFO<<"taking "<<s<<" rows for training out of "<<data.size();
    auto train_mat = data_matrix(data, conf.target_id, s);
    auto train_labl = labels_data(data, conf.target_id, s);
    return std::make_tuple(std::move(train_mat), std::move(train_labl));
}

actions::training_input preform_action(const train_run<k_fold>& , 
                                    const classifier::read_result& )
{
    LOG_DEBUG<<"processing data with k-fold";
    LOG_ERROR<<"not implemented yet";
    return actions::training_input{};
}

}   // end of namespace classifier

