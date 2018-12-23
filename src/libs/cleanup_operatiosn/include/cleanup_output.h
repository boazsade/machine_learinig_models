#pragma once
//#include "column_result.h"
#include "normalized_result.h"
#ifdef SUPPORT_FOR_MODEL_GENERATION
#   include "stats_output.h"
#   include <string> 

    struct cleanup_output
    {
        enum {
            DATA = normalized_result::DATA,
            STATS = normalized_result::STATS,
            //LOOKUP_TABLE = column_result::LOOKUP_TABLE,
            NAME = STATS + 1//LOOKUP_TABLE + 1
        };
        using result_type = normalized_result::result_type;
        using id_type = std::string;

        cleanup_output() = default;
        cleanup_output(result_type r, const id_type& i) :
            result{std::move(r)}, id(i)
        {
        }

        result_type result;
        id_type  id;
    };

    constexpr inline 
    const std::tuple_element<cleanup_output::DATA, cleanup_output::result_type>::type& 
        data(const cleanup_output& from) {
            return std::get<cleanup_output::DATA>(from.result);
    }

    constexpr inline 
    const std::tuple_element<cleanup_output::STATS, cleanup_output::result_type>::type& 
        stats(const cleanup_output& from) {
            return std::get<cleanup_output::STATS>(from.result);
    }
#if 0
    constexpr inline 
    const std::tuple_element<cleanup_output::LOOKUP_TABLE, cleanup_output::result_type>::type& 
        lookup_table(const cleanup_output& from) {
            return std::get<cleanup_output::LOOKUP_TABLE>(from.result);
    }
#endif  // disable lookup table result
    constexpr inline const cleanup_output::id_type& 
        name(const cleanup_output& from) {
            return from.id;
    }

#elif defined(SUPPORT_FOR_MODEL_USE)
    struct cleanup_output
    {
        using result_type = normalized_result::result_type;
    
        cleanup_output() = default;
        cleanup_output(result_type r) :
                result{r}
        {
        }

        result_type result;

    };

    constexpr inline const cleanup_output::result_type&
        data(const cleanup_output& from) {
            return from.result;
    }
#else
#   error "you must define either SUPPORT_FOR_MODEL_GENERATION or SUPPORT_FOR_MODEL_USE"
#endif  // SUPPORT_FOR_MODEL_GENERATION

cleanup_output make_output(normalized_result from, const std::string& id);

