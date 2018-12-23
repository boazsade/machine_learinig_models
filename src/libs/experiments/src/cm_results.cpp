#include "libs/experiments/cm_results.h"
#include <iostream>

namespace exprs
{


std::ostream& operator << (std::ostream& os, const cm_results& cmr)
{
    return os<<"true positive "<<cmr.true_pos<<", false positive "<<cmr.false_pos<<", true negative "<<
        cmr.true_neg<<", false negative "<<cmr.false_neg;
}

cm_results& cm_results::operator += (binclass::cm::result r)
{
    switch (r) {
        case binclass::cm::result::FALSE_NEG:
            ++false_neg;
            break;
        case binclass::cm::result::FALSE_POS:
            ++false_pos;
            break;
        case binclass::cm::result::TRUE_NEG:
            ++true_neg;
            break;
        case binclass::cm::result::TRUE_POS:
            ++true_pos;
            break;
        default:
            break;

    }
    return *this;
}

cm_results operator + (cm_results left, binclass::cm::result r)
{
    left += r;
    return left;
}

}   // end of namespace exprs
