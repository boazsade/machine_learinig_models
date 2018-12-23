#include "apps/model_prediction/processing_flow.h"
#include "apps/model_prediction/command_processing.h"
#include "apps/model_prediction/run_predictions.h"
#include "apps/model_prediction/schema_read.h"
#include "libs/json_parser/json_ostream.h"
#include "libs/system/exceptions.h"
#include "logging.h"


namespace prediction
{

json::ostream& operator ^ (json::ostream& ji, const output& o)
{
    return to_json(ji, o);
}

}   // end of namespace prediction

namespace
{

struct output_report
{
    explicit output_report(const prediction_results& r) :
        results{r}
    {
    }

    bool empty() const
    {
        return results.empty();
    }

    const prediction_results& results;
};

json::ostream& operator ^ (json::ostream& os, const output_report& rep)
{
    using namespace json::literals;
    if (rep.empty()) {
        os ^ "status"_n ^ "no results" ^ "msg"_n ^ "nothing to report";
    } else {
        os ^ "status"_n ^ "finish successfully";
        auto i = os ^ "values"_s;
        i ^ rep.results ^ json::_end; 
    }
    return os;
}

std::string format_output(const prediction_results& results)
{
    using namespace json::literals;

    json::output_stream root;
    auto parser = root ^ json::open;
    auto d = parser ^ "ok"_s;
    output_report report{results};

    d ^ report ^ json::_end;
    return root ^ json::str_cast;
}

}   // end of namespace 

///////////////////////////////////////////////////////////////////////////////

command_process generate_schema::operator()(const std::string& cli) const
{
    LOG_INFO<<"reading schema";
    auto sc{read_schema(cli)}; 
    if (sc.empty()) {
        LOG_ERROR<<"failed to read schema from configuration";
        throw invalid_path{"failed to read schema from the path given in configuration"};
    }
    return command_process{std::move(sc)};
}

command_process::command_process(schema_mapper s) :
            schema{std::move(s)}
{
}

std::string format_result::operator()(const prediction_results& result) const
{
    LOG_INFO<<"creating results message with output size of "<<result.size();
    const auto str{format_output(result)};
    LOG_INFO<<"output message is\n----------------------------\n"<<str<<"\n----------------------------\n";
    return str;
}

prediction_operation command_process::operator () (const std::string& cli) const
{
    LOG_INFO<<"processing command into configuration info";
    return process_command(cli, schema);
}

prediction_results prediction_engine::operator () (const prediction_operation& ops) const
{
    return run_predictions(ops);
}

command_process operator |
    (generate_schema gs,  const std::string& cli)
{
    return gs(cli);
}

prediction_operation operator | 
    (const command_process& cp, const std::string& cli)
{
    return cp(cli);
}

prediction_results operator |
    (const prediction_operation& po, prediction_engine pe)
{
    return pe(po);
}

std::string operator | 
    (const prediction_results& pr, format_result fr)
{
    return fr(pr);
}

