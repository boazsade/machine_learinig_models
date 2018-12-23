#pragma once
#include <cstdint>

struct no_such_file 
{
};

struct empty_input_file 
{
};

struct invalid_columns_count 
{
    constexpr invalid_columns_count(std::size_t exp, std::size_t act) :
        expected(exp), actual(act)
    {
    }

    const std::size_t expected;
    const std::size_t actual;
};

struct csv_parsing_error
{
};
