#pragma once
#include "lookuptable_actions.h"
#include <string>
#include <iosfwd>

namespace lookup
{
// this would be used with lookup table and not 
// by creating an internal lookup table. This is
// only applicable for class type models, hence the
// use of the model type as paramter for the action
struct lookuptable_transform
{
    using action_type = transform_action;
    using uniques_action = uniqued_function;

    lookuptable_transform() = default;

    lookuptable_transform(transform_unique act);
                    
                    
    action_type::result_type 
        operator () (const action_type::arg_type& arg) const;

    uniques_action::result_type
        operator () (const uniques_action::arg_type&) const;

    std::ostream& stream(std::ostream& os) const;

private:
    enum {
        TRANS = 0, UNIQUE
    };

    transform_unique actions;
};

lookuptable_transform::action_type::result_type
    preform_action(const lookuptable_transform& action,
            const lookuptable_transform::action_type::arg_type& arg);

lookuptable_transform::uniques_action::result_type
    preform_action(const lookuptable_transform& action,
            const lookuptable_transform::uniques_action::arg_type& arg);

std::ostream& operator << (std::ostream& os, const lookuptable_transform& lt);
std::string to_string(const lookuptable_transform& lt);

}   // end of lookup

