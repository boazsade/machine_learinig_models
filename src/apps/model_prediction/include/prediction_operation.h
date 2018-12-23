#pragma once
#include "libs/model_prediction/prediction_flow.h"
#include <boost/filesystem.hpp>
#include <iosfwd>

// holds the actual configuration to run a model
struct prediction_operation
{
    prediction::read_row read_op;
    prediction::verify_fix fix_op;
    prediction::normalized norm_op;
    prediction::load_model model;
    boost::filesystem::path source_path;
    boost::filesystem::path model_path;


    prediction_operation() = default;   // but we should not use it

    prediction_operation(prediction::read_row r,
            prediction::verify_fix f,
            prediction::normalized n,
            prediction::load_model p,
            boost::filesystem::path sp,
            boost::filesystem::path mp);
};

std::ostream& operator << (std::ostream& os, const prediction_operation& po);
