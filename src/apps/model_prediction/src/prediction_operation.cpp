#include "apps/model_prediction/prediction_operation.h"
#include <iostream>

prediction_operation::prediction_operation(prediction::read_row r,
                    prediction::verify_fix f,
                    prediction::normalized n,
                    prediction::load_model p,
                    boost::filesystem::path sp,
                    boost::filesystem::path mp) : 
    read_op{std::move(r)}, fix_op{std::move(f)}, norm_op{std::move(n)},
    model{std::move(p)}, source_path{std::move(sp)}, model_path{std::move(mp)}
{
}


std::ostream& operator << (std::ostream& os, const prediction_operation& po)
{
    return os<<"read operation "<<po.read_op<<"\nfix operation "<<po.fix_op<<
        "\nnormalization actions "<<po.norm_op<<"\nmodel "<<po.model<<"\nsource path "<<po.source_path<<", model location "<<po.model_path; 
}
