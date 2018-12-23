#pragma once
#include <string>
#include <limits>

namespace mlmodels
{

struct dtree_args
{
    enum predict_type
    {
        AUTO,
        SUM,
        MAX, 
        MASK
    };
    int cv_fold = 1;
    int max_categories = 10;
    int max_depth = 10;//std::numeric_limits<int>::max();
    int min_samples = 10;
    bool use_surrogates = false;
    predict_type prediction = AUTO;

    dtree_args() = default;
    dtree_args(int cvf, int max_cat, int max_d,
        int min_sa, bool us, predict_type p);

    static predict_type string2type(const std::string& s);

};  

}   // end of namesapce classifier

