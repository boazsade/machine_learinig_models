#pragma once
#include "params_output.h"
#include "libs/scores_evaluation/verifier_types.h"
#include "functional/functional.hpp"
#include "libs/model_datatypes/model_datatypes.h"
#include <iosfwd>

struct verify_action
{
    using result_type = poli_function<actions::experiment_input, 
                                    params_output>;
    // gets verify name and return action, note that based on the type of the member that we are passing
    // it would generate verifier - for example it may return NULL operation if this is not supported based on the 
    // type that we are holding
    using action_type = poli_function<verifier_types, result_type>;

    constexpr explicit verify_action(actions::model_type mt) :
            model_t(mt)
    {
    }

    result_type operator () (const action_type::arg_type& arg) const;
    std::ostream& print(std::ostream& os) const;
private:
    actions::model_type model_t = actions::UNKOWN_MODEL_TYPE;
};

verify_action::result_type 
    preform_action(const verify_action& va, 
            const verify_action::action_type::arg_type& arg);

std::ostream& operator << (std::ostream& os, const verify_action& va);

