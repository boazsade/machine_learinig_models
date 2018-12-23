#pragma once

#include "ROC_results.h"
#include "experiments_types.h"
#include <vector>
#include <iosfwd>

namespace exprs
{

struct ROC
{
    ROC_results results;

    ROC() = default;

    // in none classes mode, the expeced values can be any value, so
    // we are getting them as floating point values
    bool calculate(const data_array& expected,
            const data_array& predic);

    // in this mode, the expected values are classes - which means they are unique values
    // hence this is an array of integers
    bool classes_calculate(const std::vector<int>& expected,
            const data_array& predic);

    bool classes_calculate(const data_array& expected,
            const data_array& predic);

    // to help with calcualtion
    struct algo_params
    {
        algo_params() = default;
        algo_params(const data_array& ex, const data_array& p,
                value_type threshold);

        algo_params(const std::vector<int>& ex, const data_array& p,
                value_type threshold);

        data_array predictions;
        std::vector<int> partitions;
        std::size_t condition_pos = 0u;

    private:
        template<typename T>
        static std::size_t init(const std::vector<T>& ex,
                std::vector<int>& target,
                value_type threshold);
    };

private:
    template<typename T>
    bool calc(const std::vector<T>& expected,
            const data_array& predic);

};

std::ostream& operator << (std::ostream& os, const ROC& roc);

}   // end of namespace exprs
//void test_roc();
