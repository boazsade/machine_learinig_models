#include "libs/model_data/testing_input_task.h"
#include "libs/model_data/split_rows.h"
#include "libs/sources_handling/row_data_input.h"
#include "logging.h"
//#define DUMP_DATA_MAT
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

//namespace classifier
//{

actions::model_samples preform_action(const test_run<percent_split>& conf, 
                                    const classifier::read_result& data)
{
    if (classifier::empty(data)) {
        LOG_ERROR<<"invalid input for testing was passed - either failed to read data or pass empty data set";
        return actions::model_samples{};
    }
    LOG_DEBUG<<"running testing with precent split";
    auto off = classifier::spliter_at(conf.split, classifier::rows(data));          // for test we are taking the lower part of 100%
    auto s = classifier::spliter_negate(conf.split, classifier::rows(data));    // we would need to turn this to percent
    LOG_INFO<<"taking "<<s<<" rows for testing out of "<<classifier::rows(data);
    auto samples = classifier::data_matrix(data, conf.target_id, s, off);
    auto labels = classifier::labels_data(data, conf.target_id, s, off);
//    dump_data(samples, labels);
    return std::make_tuple(std::move(samples), std::move(labels));
                           
}

actions::model_samples preform_action(const test_run<k_fold>& , 
                                    const classifier::read_result& )
{
    LOG_DEBUG<<"running testing with k-fold";
    LOG_ERROR<<"this is not implemented yet";
    return actions::model_samples{};
}

//}   // end of namespace classifier

