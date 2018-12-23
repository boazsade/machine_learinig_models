#pragma once
#include "prediction_datatypes.h"
#include "prediction_row.h"
#include "functional/functional.hpp"
#include <string>
#include <iosfwd>

namespace prediction
{

struct norm_entry
{
    using result_type = data_type;
    using arg_type = row;

    using action_type = poli_function<arg_type, result_type>;

    norm_entry();

    template<typename Act>
    norm_entry(Act act) :
        action{act}, col{act.column}
    {
    }

    result_type operator () (const arg_type& all) const;

    std::ostream& stream(std::ostream& os) const;

    friend constexpr column_number
        column(const norm_entry& ne) {
            return ne.col;
    }

private:
    action_type action;
    column_number col;
};

norm_entry::result_type
    preform_action(const norm_entry& with, const norm_entry::arg_type& on);

std::ostream& operator << (std::ostream& os, const norm_entry& ne);
std::string to_string(const norm_entry& ne);

bool operator < (const norm_entry& left, const norm_entry& right);

}   // end of namespace prediction

