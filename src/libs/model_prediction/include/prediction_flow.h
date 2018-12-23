#pragma once
#include "prediction_output.h"
#include "prediction_norm.h"
#include "prediction_data.h"
#include "prediction_entries_fix.h"
#include "prediction_action.h"
#include "libs/sources_handling/map_schema.h"
#include <string>
#include <vector>
#include <iosfwd>

namespace prediction
{

// first - read the next row from the input
// note that this can fail
struct read_row
{
    using arg_type = std::istream;
    using result_type = data;

    explicit read_row(const schema_mapper& sc);

    result_type operator () (arg_type& input) const;

private:
    schema_mapper input_layout;
};
read_row::result_type read(const read_row& with,
                        read_row::arg_type& input);
std::ostream& operator << (std::ostream& os, const read_row& rr);

// next make sure that we are fixing any invalid entry
// or removing the row if this is not possible
struct verify_fix
{
    using actions_list = std::vector<fix_missing>;
    using result_type = data;
    using arg_type = result_type;

    verify_fix() = default;
    template<typename It>
    verify_fix(It from, It to) :
        actions(from, to)
    {
    }

    void assign(actions_list::value_type new_entry);

    std::ostream& stream(std::ostream& os) const;

    result_type operator () (arg_type input) const;
private:
    actions_list actions;
};
std::ostream& operator << (std::ostream& os, const verify_fix& vf);
std::string to_string(const verify_fix& vf);
verify_fix::result_type apply(const verify_fix& fix, verify_fix::arg_type what);

// Then normalized the data before we are doing the actual prediction
struct normalized
{
    using action_list = std::vector<norm_entry>;
    using result_type = data;
    using arg_type = result_type;

    template <typename It>
    normalized(It from, It to) :
        actions(from, to)
    {
        std::sort(std::begin(actions), std::end(actions));
    }

    normalized() = default;

    void assign(action_list::value_type new_entry);

    result_type operator () (arg_type arg) const;

    std::ostream& stream(std::ostream& os) const;

private:
    action_list actions;
};
std::string to_string(const normalized& n);
std::ostream& operator << (std::ostream& os, const normalized& n);
normalized::result_type norm(const normalized& with, const normalized::arg_type& what);

// this operation should be called at any time before we are doing apply_model
// This is required as a one time call so that we would have a working model that
// we can use for prediction
struct load_model
{
    using arg_type = model_load::arg_type;
    using result_type = model_load::result_type;

    template<typename Action>
    load_model(Action act) :
        action{std::move(act)}
    {
    }

    result_type operator () (const arg_type& arg) const;

    std::ostream& stream(std::ostream& os) const;
private:
    model_load action;
};

load_model::result_type
    load(const load_model& lm, const load_model::arg_type& arg);

std::ostream& operator << (std::ostream& os, const load_model& lm);
std::string to_string(const load_model& lm);

// and the last action would be to run the model on the data
// and get the results
struct apply_model
{
    using arg_type = data;
    using result_type = output;

    apply_model(predicate p);

    result_type operator () (const arg_type& arg) const;

    std::ostream& stream(std::ostream& os) const;
private:
    predicate predictor;
};
apply_model::result_type predict(const apply_model& with, const apply_model::arg_type& on);
std::ostream& operator << (std::ostream& os, const apply_model& am);

}   // end of namespace prediction

