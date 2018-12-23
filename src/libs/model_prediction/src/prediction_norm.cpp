#include "libs/model_prediction/prediction_norm.h"
#include "libs/model_prediction/prediction_norm_task.h"
#include <iostream>
#include <sstream>
#include <limits>



namespace prediction
{
norm_entry::norm_entry() :
    action{normalized_nothing}
{
}

std::ostream& norm_entry::stream(std::ostream& os) const
{
    return os<<"column: "<<col<<" with action "<<action;
}

std::ostream& operator << (std::ostream& os, const norm_entry& ne)
{
    return ne.stream(os);
}

std::string to_string(const norm_entry& ne)
{
    std::ostringstream p;
    p<<ne;
    return p.str();
}

norm_entry::result_type norm_entry::operator () (const arg_type& all) const
{
    return preform_action(action, all);
}

norm_entry::result_type
    preform_action(const norm_entry& with, const norm_entry::arg_type& on)
{
    return with(on);
}

bool operator < (const norm_entry& left, const norm_entry& right)
{
    return column(left) < column(right);
}

}   // end of namespace prediction

