#pragma once

#include "general_types.h"
#include "model_types.h"
#include <memory>

namespace mlmodels
{
    
struct knearset_impl;
struct K_nearest :
    classifier_model
{
    using model_type = std::shared_ptr<knearset_impl>;

    struct args
    {
        enum algo_type {
            BRUTE_FORCE,
            KDTREE
        };

        algo_type type = BRUTE_FORCE;
        int nearest_count = 10;

        args() = default;
        args(algo_type t, int nc);

        static algo_type string2type(const std::string& from);
    };

    K_nearest() = default;

    // when loading - you don't need to train
    static model_type load(const char* from);

    model_type train(const training_data& data,
            const class_data& classes,
            const args& a) const;

    bool predict(model_type model,
            const testing_data& data,
            testing_result& predications) const;

    class_data test(model_type model, 
            const testing_data& data, int classes) const;

    value_type predict(model_type model,
                const class_data& samples) const;

#if 0
    bool test(model_type m, const testing_data& d,
            class_data& res, class_data& dist, class_data& nr) const;
#endif
};

void save_model(K_nearest::model_type model, const char* to);

}   // end of namespace mlmodels

