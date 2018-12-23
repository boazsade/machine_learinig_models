#include "libs/ml_models/dtree_args.h"
#include "utils/missing_values.hpp"
#include <boost/assign.hpp>
#include <map>

namespace mlmodels
{

dtree_args::dtree_args(int cvf, int max_cat, int max_d,
        int min_sa, bool us, predict_type p) :
    cv_fold(cvf), max_categories(max_cat), 
    max_depth(max_d < 0 ? missing_value<int>() : max_d),
    min_samples(min_sa), use_surrogates(us), prediction(p)
{
}

dtree_args::predict_type dtree_args::string2type(const std::string& s)
{
    static const std::map<std::string, predict_type> mapper = 
        boost::assign::map_list_of("auto", AUTO)
                                  ("sum", SUM)
                                  ("max", MAX)
                                  ("mask", MASK);
    auto i = mapper.find(s);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid arg for decision trees " + s};
    } else {
        return i->second;
    }
}



}   // end of namespace mlmodels
