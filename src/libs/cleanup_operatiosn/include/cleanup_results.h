#pragma once
#ifdef SUPPORT_FOR_MODEL_GENERATION
#   include "stats_output.h"
#   include "utils/dc_lookup_table.h"
#   include <tuple>
#   include <vector>
    struct  cleanup_indices  {
        enum {
            STATS = 0, /*LOOKUP_TABLE,*/ NAME
        };
    };

    using cleanup_results_entry = std::tuple<column_info, /*lookup::table,*/ std::string>;
    using cleanup_results = std::vector<cleanup_results_entry>;
#if 0
    constexpr inline 
    const std::tuple_element<cleanup_indices::LOOKUP_TABLE, cleanup_results_entry>::type& 
        lookup_table(const cleanup_results_entry& from) {
            return std::get<cleanup_indices::LOOKUP_TABLE>(from);
    }
#endif  // disable the lookup table
    constexpr inline 
    const std::tuple_element<cleanup_indices::STATS, cleanup_results_entry>::type& 
        stats(const cleanup_results_entry& from) {
            return std::get<cleanup_indices::STATS>(from);
    }
    constexpr inline 
    const std::tuple_element<cleanup_indices::NAME, cleanup_results_entry>::type& 
        name(const cleanup_results_entry& from) {
            return std::get<cleanup_indices::NAME>(from);
    }
#elif defined(SUPPORT_FOR_MODEL_USE)
    using cleanup_results = bool;
#else
#   error "you must define either SUPPORT_FOR_MODEL_GENERATION or SUPPORT_FOR_MODEL_USE"
#endif  // SUPPORT_FOR_MODEL_GENERATION
