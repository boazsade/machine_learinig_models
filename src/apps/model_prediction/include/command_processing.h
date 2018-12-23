#pragma once
#include "prediction_operation.h"
#include "libs/sources_handling/map_schema.h"

prediction_operation process_command(const std::string& cmd,
                                    const schema_mapper& scheme);
