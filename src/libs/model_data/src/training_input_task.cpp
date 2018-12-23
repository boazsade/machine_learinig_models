#include "libs/model_data/training_input_task.h"
#include "libs/model_data/split_rows.h"
#include "logging.h"
//namespace classifier
//{
#ifdef DUMP_DATA_MAT
#include <iostream>

namespace 
{
void dump_data(const auto& data, const auto& labels)
{
    if (labels.size() != classifier::rows(data)) {
        return;
    }
    for (auto i = 0u; i < classifier::rows(data); ++i) {
        for (auto j = 0u; j < classifier::columns(data); ++j) {
            std::cout<<data[i][j]<<" ";
        }
        std::cout<<labels[i]<<std::endl;
    }
}
}
#else
namespace
{
void dump_data(const auto&, const auto& ) {}
}
#endif  // DUMP_DATA_MAT

actions::training_input preform_action(const train_run<percent_split>& conf, 
                                    const classifier::read_result& data)
{
    if (classifier::empty(data)) {
        LOG_ERROR<<"train called with invalid input - empty dataset was read or passed to the funciton";
        return actions::training_input{};
    }
    LOG_DEBUG<<"processing data with precent split for training";
    auto s = classifier::spliter_at(conf.split, classifier::rows(data));           // for test we are taking the lower part of 100%
    LOG_INFO<<"taking "<<s<<" rows for training out of "<<classifier::rows(data);
    auto train_mat = classifier::data_matrix(data, conf.target_id, s);
    auto train_labl = //classifier::transform2labels(data, conf.target_id, s);
            classifier::labels_data(data, conf.target_id, s);
    //dump_data(train_mat, train_labl);
    return std::make_tuple(std::move(train_mat), std::move(train_labl));
}

actions::training_input preform_action(const train_run<k_fold>& , 
                                    const classifier::read_result& )
{
    LOG_DEBUG<<"processing data with k-fold";
    LOG_ERROR<<"not implemented yet";
    return actions::training_input{};
}

//}   // end of namespace classifier

