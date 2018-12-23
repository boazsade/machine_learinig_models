#include "apps/generate_model/verifiers_action.h"
#include "libs/scores_evaluation/verifiers.h"
#include "libs/scores_evaluation/verifier_types.h"

//namespace detail
//{
namespace 
{

inline verify_action::action_type::result_type
    generate(verify_action::action_type::arg_type type)
{
    switch (type) {
    case MSE:
        return verify_action::action_type::result_type{MSE_test{}};
    case log_loss:
        return verify_action::action_type::result_type{logistic_loss{}};
    case AUC_ROC_t:
        return verify_action::action_type::result_type{AUC_ROC{}};
    case kalmogorov_smirnov:
        return verify_action::action_type::result_type{kalmogorov_smirnov_test{}};
    }
}
}   // end of local namespace

verify_action::action_type::result_type
    preform_action(actions::model_type mtype, verify_action::action_type::arg_type vrt)
{
    if (is_supported(vrt, mtype)) {
        return generate(vrt);
    } else {
        return verify_action::action_type::result_type{null_verifier{}};
    }
}

//}   // end of namespace detail

