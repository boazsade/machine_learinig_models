#include "apps/detect_duplicates/outliers_action.h"
#include "utils/math_utils.h"
#include "utils/missing_values.hpp"
#include <algorithm>
#include <boost/iterator/transform_iterator.hpp>
#include <map>
//#include <iostream>

namespace
{
    using algo_data = std::vector<double>;

    algo_data detect_outliers(const algo_data& inside);

    algo_data detect_outliers(const algo_data& inside)
    {
        static const double alpha_val = 0.1;
        return utils::grubbstest(inside, alpha_val);
    }
}   // end of local namespace

outlier_predicate::outlier_predicate(iterator s, iterator l) :
    to_ignore(s, l)
{
    std::sort(std::begin(to_ignore), std::end(to_ignore));
}

outlier_predicate::result_type outlier_predicate::apply_action(input_data d) const
{
    return std::make_pair(
            std::binary_search(std::begin(to_ignore), std::end(to_ignore), d.row),
            d);
}


outlier_predicate::result_type apply_action(outlier_predicate& da, input_data arg)
{
    return da.apply_action(arg);
}

outliers_detector::outliers_detector(policy_action op) :
   action(std::move(op))
{
}


outlier_predicate::result_type outliers_detector::apply_action(input_data data)
{
    if (missing_value(data.value)) {
        missings.push_back(data.row);
        return std::make_pair(true, data);     // ignore missing values
    }
    auto i = ::apply_action(action, data);
    if (!i.first) {
        legal_input.push_back(data);
    }
    return i;
}

void outliers_detector::finding(row_numbers& ol, row_numbers& missing) const
{
    auto tranformer = [](auto it) { return it.value; };
    using tranformer_type = decltype(tranformer);
    using iterator_type = boost::transform_iterator<tranformer_type, data_list::const_iterator>;
    using lookup_table = std::map<algo_data::value_type, std::size_t>;

    algo_data data{iterator_type{std::begin(legal_input), tranformer},
        iterator_type{std::end(legal_input), tranformer}
    };


    algo_data outliers{detect_outliers(data)};
    //auto lib = std::begin(legal_input);
    //auto lie = std::end(legal_input);
    // match row number with outlier that we found, this is done
    // since the actual outlier detection cannot accept row number
    // and what we are returning is outliers
    lookup_table tbl;
    std::for_each(std::begin(outliers), std::end(outliers), 
            [this, &ol, &tbl] (auto outlier) {
                auto off = tbl[outlier];
                auto lib = std::begin(legal_input);
                auto i = std::next(lib, off);
                lib = std::find_if(i, std::end(legal_input), [outlier] (auto rd) {
                        return rd.value == outlier;
                    }
                );
                if (lib != std::end(legal_input)) {   // an error
                    ol.push_back(lib->row);
                    tbl[outlier] = std::distance(std::begin(legal_input), lib) + 1u;
                    return false;
                } else {
                    return true;
                }
            }
    );
    if (missing.empty()) {  // we assume that if this is not missing then we fill it up in duplications
        missing.insert(std::end(missing), std::begin(missings), std::end(missings));
    }
}

outlier_predicate::result_type apply_action(outliers_detector& da, input_data arg)
{
    return da.apply_action(arg);
}

void finding(const outliers_detector& od, row_numbers& outlier, row_numbers& mis)
{
    od.finding(outlier, mis);
}


