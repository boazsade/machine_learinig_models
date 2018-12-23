#include "apps/run_experiments/process_input.h"
#include "apps/run_experiments/pipeline_execute.h"
#include "libs/system/exceptions.h"
#include "libs/sources_handling/row_data_input.h"
#include "libs/json_parser/json_ostream.h"
#include "libs/json_parser/json_writer.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"
#include "libs/system/error_message.h"
#include "logging.h"
#include <boost/range/algorithm/remove_if.hpp>
#include <string>
#include <iostream>
#include <fstream>

const std::string err_msg_begin = std::string{"{\"error\": {\"msg\":"};
const std::string err_msg_end = std::string{"}}"};

json::ostream& operator ^ (json::ostream& os, const report_value& rv)
{
    using namespace json::literals;
    os ^ "name"_n ^ rv.name ^ "value"_n ^ rv.value;
    return os;
}

json::ostream& operator ^ (json::ostream& os, const report_list& rl)
{
    using namespace json::literals;
    os ^ "name"_n ^ rl.name;
    auto i = os ^ "values"_s;
    i ^ rl.items ^ json::_end;
    return os;
}

namespace pipeline
{

json::ostream& operator ^ (json::ostream& os, 
        const experiment::result_type::value_type::data_type& data)
{
    using namespace json::literals;

    if (!data.items.empty()) {
        auto i = os ^ "values"_s;
        i ^ data.items ^ json::_end;
    }
    if (!data.lists.empty()) {
        auto l = os ^ "lists"_s;
        l ^ data.lists ^ json::_end;
    }
    return os;
}

json::ostream& operator ^ (json::ostream& js, 
        const experiment::result_type::value_type& exp)
{
    using namespace json::literals;
    // dont add it to the out message if we don't have data to show
    if (exp.data.lists.empty() && exp.data.items.empty()) {
        return js;
    }
    js ^ "model-name"_n ^ exp.model ^ "experiment-name"_n ^ exp.experiment;
    auto ds = js ^ "data-set"_s;
    ds ^ exp.data ^ json::_end;
    return js;
}

}   // end of namespace pipeline

struct success_report {
    const std::string msg;
    const std::string details;
    pipeline::experiment::result_type expers_result;

    explicit success_report(const std::string& path,
            pipeline::experiment::result_type rt) :
        msg("finish successfully"), 
        details("successfully finish processing data from " + path),
        expers_result(std::move(rt))
    {
    }
};

void report_errors(const error_report& errors, bool tolog = true);
void setup_log(const char* name);
void report_success(pipeline::experiment::result_type&& result,
        const std::string& sp);
json::ostream& operator ^ (json::ostream& js, const error_report& err);
json::ostream& operator ^ (json::ostream& js, const success_report& err);


void setup_log(const char* name)
{
#ifdef NDEBUG
    static const sl::severity_level log_severity = sl::info;
#else 
    static const sl::severity_level log_severity = sl::debug;
#endif // NDEBUG
    static const char* LOG_DIR_NAME = "logs";
    sl::default_log(log_severity, LOG_DIR_NAME, name);
}

void report_errors(const error_report& errors, bool log)
{
    if (log) {
        LOG_ERROR<<"critical error: "<<errors;
    }
    std::cerr<<to_json(errors)<<std::endl;
}

int main(int argc, char** argv)
{
    try {
        setup_log("run_experiment");
    } catch (...) {
        report_errors(error_report{"log error", "failed to initialized logs"}, false);        
        return -1;
    }
    if (argc != 2) {
        report_errors(error_report{"invalid args",  "input [file|JSON from standard input]"});
        return -1;
    }
    LOG_INFO<<argv[0]<<" called with args list;\n"<<argv[1]<<std::endl;
    try {
        LOG_INFO<<"start processing\ninput arg:\n"<<argv[1]<<"\n";
        auto conf = process_input(argv[1]); 
        auto data_source = pipeline::source_path(conf).string();
        LOG_INFO<<"configuration:\n"<<conf;
        LOG_INFO<<"start running experiments";
        pipeline::experiment exp;
        auto expers_result = exp.run(std::move(conf));
        if (expers_result.empty()) {
            report_errors(error_report{"not experiments results", "failed to run experiments to completion/failed to produce results"});
            return -2;
        } else {
            report_success(std::move(expers_result), data_source);
        }
    } catch (const invalid_conf& ic) {
        report_errors(error_report{"invalid configuration", ic.what()});
        return -1;
    } catch (const json_error& je) {
        report_errors(error_report{"json error", je.what()});
        return -1;
    } catch (const std::runtime_error& re) {
        report_errors(error_report{"runtime error", re.what()});
        return -1;
    } catch (const malformat_input& e) {
        report_errors(error_report{"malformatted column input", e.what()});
        return -1;
    } catch (const std::exception& e) {
        LOG_ERROR<<"critical error: "<<e.what();
        report_errors(error_report{"critical error", e.what()});
        return -1;
    } catch (...) {
        LOG_ERROR<<"error while trying to run experience - unknown";
        return -1;
    }
    LOG_INFO<<"finish successfully to build models and test them";
    return 0;
}

void report_success(pipeline::experiment::result_type&& result,
        const std::string& source_path)
{
    using namespace json::literals;

    result.erase(boost::remove_if(result, [](auto& entry) { return entry.data.lists.empty() && entry.data.items.empty();}), result.end());
    if (result.empty()) {
        report_errors(error_report{"no valid results", "failed to generate valid entries from input data"});
        return;
    }
    LOG_INFO<<"successfully finish to run train --> tests --> experiments and got "<<result.size()<<"results";
    LOG_INFO<<"results are\n";
    unsigned int ri = 0;
    for (const auto& r : result) {
        LOG_INFO<<"["<<ri++<<"]: "<<r;
    }
    success_report sr{source_path, std::move(result)};
    json::output_stream root;
    auto parser = root ^ json::open;
    auto d = parser ^ "ok"_s;
    d ^ sr ^ json::_end;
    auto msg = root ^ json::str_cast;
    std::cout<<msg<<std::endl;
}

json::ostream& operator ^ (json::ostream& js, const success_report& ok)
{
    using namespace json::literals;
    js ^ "status"_n ^ ok.msg ^ "msg"_n ^ ok.details;
    auto d = js ^ "details"_s;
    d ^ ok.expers_result ^ json::_end;
    return js;
}

