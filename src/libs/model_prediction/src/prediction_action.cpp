#include "libs/model_prediction/prediction_action.h"
#include <sstream>
#include <iostream>
#include <limits>
//#error Must fix DP-760
namespace prediction
{

namespace
{
bool good_prediction(predicate::action_type::result_type value)
{
    return value != std::numeric_limits<predicate::action_type::result_type>::max() && 
        not (std::isinf(value) || std::isnan(value));
}
}   // end of namespace


std::ostream& predicate::stream(std::ostream& os) const
{
    return os<<"prediction with action "<<action;
}

std::ostream& operator << (std::ostream& os, const predicate& p)
{
    return p.stream(os);
}

std::string to_string(const predicate& p)
{
    std::ostringstream s;
    s<<p;
    return s.str();
}

predicate::result_type predicate::operator ()(const arg_type& r) const
{
    // we need to generate a data to the actual model to use as this has
    // the data and the row number
    //std::cerr<<"running prediction on "<<r<<std::endl;
    action_type::arg_type data(r.begin(), r.end());
    //std::cerr<<" finish making data for model"<<std::endl;
    //std::cerr<<"running action "<<action<<" for data "<<data.size()<<std::endl;
    auto res = preform_action(action, data);
    //std::cerr<<"finish applying model on data"<<std::endl;
    if (good_prediction(res)) {
    //    std::cerr<<"returning good results for model"<<std::endl;
       return result_type{index(r), res};
    } else {
    //    std::cerr<<"return bad result for model"<<std::endl;
        return result_type{index(r), status::MODEL_ERROR};
    }
}


model_load::result_type model_load::operator () (const arg_type& from) const
{
    return preform_action(action, from);
}

std::ostream& model_load::stream(std::ostream& os) const
{
    return os<<"load model using "<<action;
}

std::ostream& operator <<(std::ostream& os, const model_load& ml)
{
    return ml.stream(os);
}

std::string to_string(const model_load& ml)
{
    std::stringstream p;
    p<<ml;
    return p.str();
}

predicate::result_type
    preform_action(const predicate& p, const predicate::arg_type& arg)
{
    return p(arg);
}

model_load::result_type
    preform_action(const model_load& ml, const model_load::arg_type& from)
{
    return ml(from);
}

}   // end of namespace prediction

