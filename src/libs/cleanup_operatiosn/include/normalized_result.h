#pragma once
#include "fix_missing.h"
#ifdef SUPPORT_FOR_MODEL_GENERATION

#   include "stats_output.h"
#   include <tuple>

    struct normalized_result
    {
        using result_type = std::tuple<fix_missing::result_type, column_info>;

        enum {
            DATA = 0,
            STATS = 1,
        };

        normalized_result() = default;
        explicit normalized_result(const result_type& r) : 
            output{r}
        {
        }

        result_type output;
    };

    constexpr inline
    const std::tuple_element<normalized_result::DATA, normalized_result::result_type>::type&
        data(const normalized_result& from) {
            return std::get<normalized_result::DATA>(from.output);
    }

    constexpr inline
    const std::tuple_element<normalized_result::STATS, normalized_result::result_type>::type&
        stats(const normalized_result& from) {
            return std::get<normalized_result::STATS>(from.output);
    }


#elif defined (SUPPORT_FOR_MODEL_USE)

    struct normalized_result
    {
        using result_type = fix_missing::result_type;

        normalized_result() = default;
        explicit normalized_result(const result_type& r) :
            output{r}
        {
        }

        result_type output;
    };
    constexpr inline const normalized_result::result_type&
        data(const normalized_result& from) {
            return from.output;
    }
#else
#   error "you must define either SUPPORT_FOR_MODEL_GENERATION or SUPPORT_FOR_MODEL_USE"
#endif  // SUPPORT_FOR_MODEL_GENERATION
