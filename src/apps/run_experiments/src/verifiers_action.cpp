#include "apps/run_experiments/verifiers_action.h"
#include "verifier_action_generator.hpp"


verify_action::action_type::result_type 
    preform_action(const verify_action& va, 
            const verify_action::action_type::arg_type& arg)
{
    return va(arg);
}

std::ostream& operator << (std::ostream& os, const verify_action& va)
{
    return va.print(os);
}

std::ostream& verify_action::print(std::ostream& os) const
{
    return os<<"verifirier action generation for "<<model_t;
}

verify_action::action_type::result_type 
    verify_action::operator()(const action_type::arg_type& arg) const
{
    return preform_action(model_t, arg);
}
