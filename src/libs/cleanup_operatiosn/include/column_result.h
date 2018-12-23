#pragma once
#include "normalized_result.h"

#ifdef SUPPORT_FOR_MODEL_GENERATION
#   include "stats_output.h"
#   include "utils/dc_lookup_table.h"
#   include <tuple>

    struct column_result
    {
        enum {
            DATA = 0,
            STATS = 1,
            LOOKUP_TABLE = 2
        };
        using data_type = std::tuple_element<normalized_result::DATA, normalized_result::result_type>::type;
        using stats_type = std::tuple_element<normalized_result::STATS, normalized_result::result_type>::type;
        using result_type = std::tuple<data_type, stats_type, lookup::table>;

        column_result() = default;
        explicit column_result(const result_type& r) : 
            output{r}
        {
        }
        result_type output;
    };
    constexpr inline
    const std::tuple_element<column_result::DATA, column_result::result_type>::type&
        data(const column_result& from) {
            return std::get<column_result::DATA>(from.output);
    }

    constexpr inline
    const std::tuple_element<column_result::STATS, column_result::result_type>::type&
        stats(const column_result& from) {
            return std::get<column_result::STATS>(from.output);
    }
    constexpr inline
    const std::tuple_element<column_result::LOOKUP_TABLE, column_result::result_type>::type&
        lookup_table(const column_result& from) {
            return std::get<column_result::LOOKUP_TABLE>(from.output);
    }

    inline column_result 
        make_results(column_result::data_type&& d, column_result::stats_type&& s, lookup::table&& t) {
                return column_result{std::make_tuple(std::move(d), std::move(s), std::move(t))};
    }

    inline column_result 
        make_results(normalized_result s, lookup::table&& t) {
                return column_result{std::tuple_cat(s.output, std::make_tuple(t))};
    }

#elif defined (SUPPORT_FOR_MODEL_USE)
    using column_result = normalized_result;
#else
#   error "you must define either SUPPORT_FOR_MODEL_GENERATION or SUPPORT_FOR_MODEL_USE"
#endif  // !SUPPORT_FOR_MODEL_USE && !SUPPORT_FOR_MODEL_GENERATION
