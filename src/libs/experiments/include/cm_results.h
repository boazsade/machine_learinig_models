#pragma once
#include "confusion_matric.h"
#include <iosfwd>

namespace exprs
{

struct cm_results
{
    unsigned int true_pos = 0u;
    unsigned int false_pos = 0u;
    unsigned int true_neg = 0u;
    unsigned int false_neg = 0u;

    constexpr cm_results() = default;
    constexpr cm_results(unsigned int tp, unsigned int fp,
            unsigned int tn, unsigned int fn) :
                true_pos{tp}, false_pos{fp}, true_neg{tn}, false_neg{fn}
    {
    }

    cm_results& operator += (binclass::cm::result r);
};

cm_results operator + (cm_results left, binclass::cm::result r);
std::ostream& operator << (std::ostream& os, const cm_results& cmr);

}   // end of namespace exprs

