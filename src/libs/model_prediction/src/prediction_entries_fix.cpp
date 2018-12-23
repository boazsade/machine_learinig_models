#include "libs/model_prediction/prediction_entries_fix.h"
#include "libs/model_prediction/prediction_row.h"
#include <iostream>

namespace prediction
{

std::ostream& operator << (std::ostream& os, const fix_missing& fm)
{
    return os<<to_string(fm);
}

std::string to_string(const fix_missing& fm)
{
    using namespace std::string_literals;

    return to_string(static_cast<const basic_contval_fix&>(fm)) 
        +  to_string(fm.column);
}

void replace(row& r, const fix_missing& fixer)
{
    r[fixer.column.value] = fixer.value;
}


}   // end of namespace prediction

