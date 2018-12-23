#pragma once
#include "prediction_operation.h"
#include "run_predictions.h"
#include "libs/sources_handling/map_schema.h"

struct command_process;
constexpr struct generate_schema
{
    constexpr generate_schema() = default;

    command_process operator () (const std::string& cli) const;
} read_scheme = generate_schema{};

struct command_process
{
    explicit command_process(schema_mapper sm);

    prediction_operation operator () (const std::string& cli) const;

private:
    schema_mapper schema;
};

constexpr struct prediction_engine
{
    constexpr prediction_engine() = default;

    prediction_results operator () (const prediction_operation& ops) const;

} predicator = prediction_engine{};

constexpr struct format_result
{
    constexpr format_result() = default;

    std::string operator () (const prediction_results& result) const;
} formatter = format_result{};

command_process operator |
    (generate_schema gs,  const std::string& cli);

prediction_operation operator | 
    (const command_process& cp, const std::string& cli);

prediction_results operator |
    (const prediction_operation& po, prediction_engine pe);

std::string operator | 
    (const prediction_results& pr, format_result fr);

