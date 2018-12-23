#ifndef SUPPORT_FOR_MODEL_GENERATION
#   define SUPPORT_FOR_MODEL_GENERATION
#endif  // SUPPORT_FOR_MODEL_GENERATION
#include "libs/transform_action/mapvalues_task.h"
#include "libs/transform_action/map_values_conf.h"
#include "libs/sources_handling/row_data_input.h"
#include "utils/dc_lookup_table.h"
#include "logging.h"
#include <boost/range/algorithm/transform.hpp>

namespace lookup
{
namespace 
{

tablegen_function::result_type 
    default_case(const tablegen_function::arg_type& )
{
    return 
        make_trasform_func(dont_trasform_op); 
}


table make_lookup_table(const mlmodels::class_data& from)
{
    return map(std::begin(from), std::end(from));
}

table make_table(const /*lookup::func::*/tablegen_function::arg_type& from, 
        const column_data::id_type& id)
{
    auto lbl_col = classifier::labels_data(from, id, classifier::rows(from));
    return make_lookup_table(lbl_col);
}

transform_action::result_type 
    convert(transform_action::arg_type from, const table& transfomer) 
{
    using target_type = transform_action::arg_type::value_type;
    if (!from.empty()) {
        
        boost::transform(from, from.begin(), [&transfomer](auto val) {
                    return static_cast<target_type>(mapped(transfomer, val));
                }
        );
    }
    return from;
}

}   // end of local namespace

transform_action::result_type
    preform_action(const lookup::table& table, const transform_action::arg_type& column)
{
    return convert(column, table);
}


tablegen_function::result_type preform_action(
        const map_values_conf& action, const tablegen_function::arg_type& arg)
{
    LOG_INFO<<"creating lookup table from input data of size "<<classifier::rows(arg)<<" for column "<<action.column;

    if (classifier::empty(arg)) {
        LOG_INFO<<"failed to create values to integers mapping - data is empty";
        return default_case(arg);
    } else if (action.classifier()) {
        LOG_DEBUG<<"creating lookup table for classifier model type";
        return  make_trasform_func(make_table(arg, action.column));
    } else {
        LOG_DEBUG<<"no need for table lookup - we are running in regression mode";
        return default_case(arg);
    }
}

uniqued_function::result_type 
    preform_action(uniqued_function::result_type val, const uniqued_function::arg_type&)
{
    return val;
}

} // end of namespace lookup
