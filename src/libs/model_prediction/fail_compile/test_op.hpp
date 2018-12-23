#pragma once
#include "libs/model_prediction/prediction_row.h"

namespace prediction
{
    inline bool operator == (const row& left, const row& right)
    {
        if (left.size() == right.size()) {
            unsigned int i = 0;
            for (; i < left.size() && left[i] == right[i]; ++i) {
            }
            return i == left.size();
        }
        return false;
    }

    inline bool operator != (const row& left, const row& right) {
        return not (left == right);
    }
}

