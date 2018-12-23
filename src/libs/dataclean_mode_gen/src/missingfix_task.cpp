#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "utils/interpolations.h"
#include "utils/cubic_iterpolation.h"
#include "logging.h"

namespace
{
    cubic::spline_data make_spline(fix_missing::arg_type column_data, 
            const cubical_fix::row_numbers& missing)
    {
        auto new_end = filter_out(begin(column_data), end(column_data), 
                std::begin(missing), std::end(missing));
        return cubic::spline_data{row_nums_transform(begin(column_data), new_end),
                            transform_slice(begin(column_data), new_end)
            };
    }

    using function_type = math::fx_list_type::value_type (*)(const math::fx_list_type&, 
            const math::x_list_type&, math::x_list_type::value_type);
    template<typename Iter, typename ArgRes>
    void fix_na(ArgRes& arg, function_type fix_func, Iter row_num, unsigned int slice_size)
    {
        auto missing = find(arg, *row_num);
        if (missing == end(arg)) {
            LOG_INFO<<"cannot fix "<<*row_num<<" as we failed to find it in rows list";
            return; // we cannot fix this one
        }
        auto s = slice(arg, row_num, slice_size);
        if (s.size() < slice_size) {
            LOG_ERROR<<"cannot fix missing value at row "<<*row_num<<" as the area around it "<<s.size()<<" is too small";
            return; // we cannot use this..
        }
        const auto vals = transform_slice(std::begin(s), std::end(s));
        const auto cords = row_nums_transform(std::begin(s), std::end(s));
        auto calc = fix_func(vals, cords, *row_num);
        LOG_DEBUG<<"fixing missing value "<<missing->value<<" with "<<calc;
        missing->value = calc;
    }
}   // end of local namespace

fix_missing::result_type preform_action(const cubical_fix& cubic_action, fix_missing::arg_type arg)
{
    if (size(arg) < 2) {
        LOG_INFO<<"data set input is to small, we cannt fix it";
        return arg;
    }
    //constexpr unsigned int cub_size = 2;
    LOG_DEBUG<<"apply missing fix with cubic algorithm on data size "<<size(arg)<<" by "<<cubic_action;
    const auto spline = make_spline(arg, cubic_action.invalid_rows);
    for (auto row : cubic_action.invalid_rows) {
        auto i = find(arg, row);
        if (i != end(arg)) {
            i->value = cubic::interpole(spline, row);
        }
    }
    return arg;
}

fix_missing::result_type preform_action(const linear_fix& linear_action, fix_missing::arg_type arg)
{
    if (size(arg) < 2) {
        LOG_DEBUG<<"data set input is to small, we cannt fix it";
        return arg;
    }
    constexpr unsigned int slice_size = 1;
    LOG_DEBUG<<"apply missing fix with linear algorithm on data size "<<size(arg)<<" by "<<linear_action;
    //std::for_each(linear_action.invalid_rows.begin(), linear_action.invalid_rows.end(), [&arg, slice_size] (auto row_num) {
    for (auto invalid_begin = linear_action.invalid_rows.begin(); invalid_begin !=  linear_action.invalid_rows.end(); ++invalid_begin) {
        fix_na(arg, utils::linear_interpolation, invalid_begin, slice_size);
    }
    return arg;
}

fix_missing::result_type preform_action(const kneighborhood_fix& action, fix_missing::arg_type arg)
{
    if (size(arg) < 2) {
        LOG_INFO<<"data set input is to small, we cannt fix it";
        return arg;
    }
    auto slice_size = action.n_count;
    LOG_DEBUG<<"apply missing fix with kneighborhood algorithm on data size "<<size(arg)<<" by "<<action;
    //std::for_each(action.invalid_rows.begin(), action.invalid_rows.end(), [&arg, slice_size] (auto row_num) {
    for (auto invalid_begin = action.invalid_rows.begin(); invalid_begin !=  action.invalid_rows.end(); ++invalid_begin) {
        fix_na(arg, utils::K_neighborhood, invalid_begin, slice_size);
    }
    return arg;
}

