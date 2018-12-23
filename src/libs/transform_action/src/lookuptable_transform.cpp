#include "libs/transform_action/lookuptable_transform.h"
#include <iostream>
#include <boost/range/algorithm/transform.hpp>
#include <sstream>

namespace lookup
{

std::ostream& operator << (std::ostream& os, const lookuptable_transform& lt)
{
    return lt.stream(os);
}

std::string to_string(const lookuptable_transform& lt)
{
    std::ostringstream p;
    p<<lt;    
    return p.str(); 
}


lookuptable_transform::action_type::result_type
    preform_action(const lookuptable_transform& action,
            const lookuptable_transform::action_type::arg_type& arg)
{
    return action(arg);
}

lookuptable_transform::lookuptable_transform(transform_unique act) :
                                 actions{std::move(act)}
{
}

lookuptable_transform::action_type::result_type 
        lookuptable_transform::operator () (const action_type::arg_type& arg) const
{
    return preform_action(std::get<TRANS>(actions), arg);
}

lookuptable_transform::uniques_action::result_type
    lookuptable_transform::operator () (const uniques_action::arg_type& arg) const
{
    return preform_action(std::get<UNIQUE>(actions), arg);
}

std::ostream& lookuptable_transform::stream(std::ostream& os) const
{
    //static const std::string label = "lookuptable_transform: ";
    return os<<"{\n"<<"\"lookup\": \n"<<std::get<TRANS>(actions)<<"\n,\"uniques\": "<<std::get<UNIQUE>(actions)<<"\n}";
}

lookuptable_transform::uniques_action::result_type
    preform_action(const lookuptable_transform& action,
            const lookuptable_transform::uniques_action::arg_type& arg)
{
    return action(arg);
}

}   // end of namespace lookup

