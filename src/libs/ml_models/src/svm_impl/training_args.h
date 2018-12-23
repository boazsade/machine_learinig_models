#pragma once
#include <iosfwd>

namespace mlmodels
{
namespace svm
{

struct base_train_params
{
    double cache_size = 100;    // in MB
    double stop_crit = 1e-3;	// stopping criteria 
    bool shrinking = true;	// use the shrinking heuristics
    bool  probability = true;  // do probability estimates 

    constexpr base_train_params() = default;
    constexpr base_train_params(double cs, double sc,
                            bool es, bool ep) :
            cache_size{cs}, stop_crit{sc}, shrinking{es}, probability{ep}
    {
    }
};
std::ostream& operator << (std::ostream& os, const base_train_params& btp);


}   // end of namesapce svm
}   // end of mlmodels

