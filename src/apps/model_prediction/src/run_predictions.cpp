#include "apps/model_prediction/run_predictions.h"
#include "libs/model_prediction/prediction_flow.h"
#include "libs/system/exceptions.h"
#include "logging.h"
#include <algorithm>
#include <iterator>
#include <fstream>


namespace 
{

constexpr struct verify_source {} src_ver = verify_source{};
constexpr struct verify_model_source{} model_src_ver = verify_model_source{};
constexpr struct model_loading{} open_model = model_loading{};
constexpr struct data_loading{} open_src = data_loading{};
struct do_read{
    using value_type = prediction::read_row;
    explicit do_read(const prediction_operation& rop) :
        read_op{rop.read_op}
    {
    }

    value_type::result_type operator () (value_type::arg_type& arg) const
    {
        return prediction::read(read_op, arg);
    }

    const value_type& read_op;
}; 
struct do_fix {
    using value_type = prediction::verify_fix;

    explicit do_fix(const prediction_operation& op) :
            fix_op{op.fix_op}
    {
    }

    value_type::result_type operator () (value_type::arg_type arg) const
    {
        return prediction::apply(fix_op, arg);
    }

    const value_type& fix_op;
};

struct do_normalized {
    using value_type = prediction::normalized;

    explicit do_normalized(const prediction_operation& op) :
        norm_op{op.norm_op}
    {
    }

    value_type::result_type operator () (const value_type::arg_type arg) const
    {
        return prediction::norm(norm_op, arg);
    }

    const value_type& norm_op;
};

struct do_predicte {
    using value_type = prediction::apply_model;

    explicit do_predicte (const value_type& op) :
        pred_op{op}
    {
    }

    value_type::result_type operator () (const value_type::arg_type& arg) const
    {
        return prediction::predict(pred_op, arg);
    }

    const value_type& pred_op;
};

inline
const prediction_operation& operator | (const prediction_operation& po, verify_source) {
    if (boost::filesystem::exists(po.source_path)) {
        LOG_INFO<<"source file "<<po.source_path<<" found";
        return po;
    }
    LOG_ERROR<<"source file not found "<< po.source_path;
    throw invalid_path{"source file not found " + po.source_path.string()};
}

inline 
bool operator | (const prediction_operation& po, verify_model_source) {
    if (boost::filesystem::exists(po.model_path)) {
        LOG_INFO<<"model file "<<po.model_path<<" found";
        return true;
    }
    LOG_ERROR<<"model file not found "<< po.model_path;
    throw invalid_path{"model file not found " + po.model_path.string()};
}

inline prediction::apply_model operator |
    (model_loading, const prediction_operation& op) {
        LOG_INFO<<"loading model from "<<op.model_path;
        return prediction::apply_model{prediction::predicate{prediction::load(op.model, op.model_path)}};
}

inline do_read::value_type::result_type operator | 
    (const do_read& ro, std::istream& from) {
        return ro(from);
}

inline do_fix::value_type::result_type operator |
    (do_read::value_type::result_type arg, const do_fix& fix) {
        return fix(arg);
}

inline do_normalized::value_type::result_type operator |
    (do_fix::value_type::result_type arg, const do_normalized& normalizer) {
        return normalizer(arg);
}

inline do_predicte::value_type::result_type operator |
    (do_normalized::value_type::result_type arg, const do_predicte& predicate) {
        return predicate(arg);
}

prediction::output
    do_prediction(std::istream& src, const prediction::apply_model& model, 
                  const prediction_operation& op);

struct execution_policy
{
    execution_policy(prediction_operation op,  prediction::apply_model m) :
       ops{std::move(op)}, model{std::move(m)}
    {
    }

    prediction::output operator ()(std::istream& s) const
    {
        return do_prediction(s, model, ops);
    }

    prediction_operation ops;
    prediction::apply_model model;
};

///////////////////////////////////////////////////////////////////////////////
// this function would tie it all
prediction::output
    do_prediction(std::istream& src, const prediction::apply_model& model, 
                  const prediction_operation& op)
{
    // the follow 
    // read_row --> verify_fix --> normalized --> aplly_model
    return do_read{op} | src | do_fix{op} | do_normalized{op} | do_predicte{model};
}

/////////////////////////////////////////////////////////////////////////////
// general case function that would transform from input to some target type
// using some source
template<typename Input, typename Function, typename Target, typename Pred>
Target transform_if(Input& src, Function&& func, Target target, Pred if_op) {
    while (src) {
        auto r = func(src);
        if (if_op(r)) {
            target = r;
        }
    }
    return target;
}

}   // end of local namespace

///////////////////////////////////////////////////////////////////////////////

prediction_results run_predictions(const prediction_operation& ops)
{
    LOG_INFO<<"running predictions with configuration of\n---------------------------------------\n"<<ops<<"\n---------------------------------------\n";
    auto b = ops | src_ver | model_src_ver;
    if (b) {
        LOG_INFO<<"successfully verify configuration";
    }
    LOG_INFO<<"openning source data "<<ops.source_path<<" for reading";
    std::ifstream src{ops.source_path.string()};
    if (src) {
        LOG_INFO<<"successfully opened source file "<<ops.source_path<<" for reading";
        // we need to read the first line which is the titles and ignore it..
        do_read{ops} | src;
    } else {
        LOG_ERROR<<"failed to open source file "<<ops.source_path<<" for reading";
        throw invalid_path{"failed to open source file " + ops.source_path.string() + " for reading"};
    }
    auto model = open_model | ops;
    prediction_results output;
    LOG_INFO<<"start processing rows from input";
    //execution_policy executer{std::move(ops), std::move(model)};    
    //transform_if(src, executer, std::back_inserter(output), prediction::valid_output);
    //[&model, &ops](std::istream& s) { return do_prediction(s, model, ops); }, std::back_inserter(output)); 
    transform_if(src, [&ops, &model](auto& s) { return do_prediction(s, model, ops); }, std::back_inserter(output),  prediction::valid_output);
    return output;
}

