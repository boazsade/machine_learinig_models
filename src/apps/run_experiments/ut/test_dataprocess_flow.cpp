#include <boost/test/unit_test.hpp>
#include "data_for_testing.h"
#include "columns4testing.h"
#include "apps/run_experiments/pipeline_flow.h"
#include "apps/run_experiments/processing_pipeline.h"
#include "apps/run_experiments/actions_pipeline.h"
#include "libs/sources_handling/row_data_input.h"
#include "libs/classifier_models_actions/classifer_model_interface.h"
#include "libs/classifier_models_actions/classifers_models.h"
#include "libs/classifier_models_actions/classifersmodels_task.h"
#include "libs/transform_action/map_values_conf.h"
#include "libs/transform_action/map_values_op.h"
#include "libs/transform_action/lookuptable_transform.h"
#include "libs/transform_action/mapvalues_default_task.h"
#include "libs/transform_action/mapvalues_task.h"
#include "utils/algorithms.hpp"
#include <string>
#include <vector>

namespace 
{
pipeline::train_data make_train_data(actions::model_type type)
{
    return pipeline::train_data {
        pipeline::train_data::trainers{
            pipeline::config::trainers::value_type{
                classifier::SVM_train{},
                type
            }  // which model is not important..
        }, 
        pipeline::train_data::experiments{
            pipeline::train_data::experiments::value_type{AUC_ROC_t}
        },
        pipeline::train_data::source_policy{
            pipeline::config::source_policy{percent_split{
                flowut::split_size, flowut::classes_column_id}
            } // the name of class column
        }
    }; 
}

template<typename Func>
void test_model_flow(actions::model_type type, 
            const char* message, Func eval_func)
{
    const auto td{make_train_data(type)};
    const auto ts{*td.begin()};
    // we also need to build a lookup table out of the data that we have..
    const lookup::map_values_conf conf{flowut::classes_column_id, type};
    //const auto lookup_action{conf};
    const auto data{flowut::data4test()};
    const auto lookup_trans = preform_action(conf, data);
    const auto train_output = pipeline::train(ts, data,  lookup_trans);
    //std::cout<<"table lookup action "<<lookup_action<<" transform action "<<lookup_trans<<std::endl;
    // after this we need to ensure that we don't have the same data 
    // as the one that we have using the call here
    auto expr_data = pipeline::test(train_output, data, lookup_trans);
    // now we would like to make sure that we have the data for the experiment
    // after transform not the same as the data one that we have before it
    const auto& labels = actions::measures(expr_data);
    pipeline::train_data::source_policy data_extract{
                pipeline::config::source_policy{
                    percent_split{flowut::split_size, flowut::classes_column_id}
                } // the name of class column
    };
    auto test_data = preform_action(data_extract.test_reader, data);
    const auto orig_labels = actions::labels_data(test_data);
    BOOST_TEST_REQUIRE(utils::size(labels) == utils::size(orig_labels));
    // we need to verify that we don't have any of the data same as the one that
    // we read into it since we transform it to be classes
#if 0
    std::cout<<"data before changes\n";
    out_range(std::cout, orig_labels);
    std::cout<<"\ndata after change \n";
    out_range(std::cout, labels);
    std::cout<<std::endl;
    return;
#endif
    for (auto v : labels) {
        //BOOST_TEST((std::none_of(std::begin(orig_labels), std::end(orig_labels), [v](auto s) { return v == s; }) == equal), 
        BOOST_TEST(eval_func(orig_labels, v), 
                message << "expect to find "<<v<<" in the original data");
    }
}

}   // end of local namespace

BOOST_AUTO_TEST_CASE(verify_load_data)
{
    // make sure that we are able to generate data correctly from array
    // one model - SVM, one experiment AUC and we are spliting by precent (the default)
    const auto td{make_train_data(actions::CLASSIFIER_MODEL)};
    BOOST_TEST_REQUIRE(td.trainers_count() == 1u);
    BOOST_TEST_REQUIRE(td.expers_count() == 1u);
    BOOST_TEST_REQUIRE((td.begin() != td.end()), "we are expecting the have none empty range in the iterations");
    BOOST_TEST_REQUIRE(flowut::data4test().size() == utils::size(flowut::column_idx));
}

BOOST_AUTO_TEST_CASE(verify_classifier_model)
{
    test_model_flow(actions::CLASSIFIER_MODEL, "we don't ", 
                    [](const auto& range, auto v) {
                        return std::none_of(std::begin(range), std::end(range), [v] (auto a) {
                                    return v == a;
                                });
                    }
            );
}

BOOST_AUTO_TEST_CASE(verify_regression_model)
{
    test_model_flow(actions::REGRESSION_MODEL, "we do ", 
                     [] (const auto& range, auto v) {
                        return std::any_of(std::begin(range), std::end(range), [v](auto a) {
                                        return v == a;
                                    });
                                }
                        );
}

