#ifndef SUPPORT_FOR_MODEL_USE
#   define SUPPORT_FOR_MODEL_USE
#endif  // SUPPORT_FOR_MODEL_USE
#include "logging.h"
#include "libs/cleanup_operatiosn/dataclean_conf.h"
#include "libs/cleanup_operatiosn/command_processing.h"
#include "libs/cleanup_operatiosn/cleanup_tasks.h"
#include "libs/json_parser/json_ostream.h"
#include "libs/json_parser/json_writer.h"
#include "libs/json_parser/json_istream.h"
#include "libs/system/exceptions.h"
#include "libs/system/error_message.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <vector>

struct success_report {
    const std::string msg;
    const std::string details;

    explicit success_report(const std::string& path) :
        msg("finish successfully"), 
        details("successfully finish processing data from " + path)
    {
    }
};

#ifdef NDEBUG
    sl::severity_level log_severity = sl::info;
#else 
    sl::severity_level log_severity = sl::debug;
#endif // NDEBUG
std::ostream& operator << (std::ostream& os, const schema_type& s);
json::ostream& operator ^ (json::ostream& js, const success_report& err);
void report_success(const success_report& errors);
int report_error(error_report&& err);
dataclean_conf process_cmd(const std::string& arg);
void setup_log();
void log_print_schema(const schema_type& s);
cleanup_results clean(const dataclean_conf& withconf);


int main(int argc, char** argv)
{
    if (argc != 2) {
        LOG_ERROR<<"invalid command line args for "<<argv[0]<<" should have passed with json";
        return report_error(error_report{"invalid args", "usage: <json input> - either as file or on command line"});
    }

    try {
        setup_log();
        LOG_INFO<<"\n------------------------------------------------------------------------------\n"
            <<"   Starting "<<argv[0]<<" with \n"<<argv[1]
            <<"\n------------------------------------------------------------------------------\n";
        auto conf = process_cmd(argv[1]);
        LOG_INFO<<"finish configuration - starting processing";
        LOG_DEBUG<<"input:\n"<<conf;
        auto report = clean(conf);
        if (report) {
            report_success(success_report{conf.source_location});
        } else {
            report_error(error_report{"failed to execute", "either invalid configuration or failed to run the given configuration"});
        }
    } catch (const invalid_path& ip) {
        LOG_ERROR<<"invalid path given for processing input at "<<argv[1];
        return report_error(error_report{"invalid input path", ip.what()});
    } catch (const json_error& je) {
        LOG_ERROR<<"failed to process json input "<<argv[1];
        return report_error(error_report{"json parsing error", je.what()});
    } catch (const std::exception& e) {
        LOG_ERROR<<"run time error: "<<e.what();
        return report_error(error_report{"error", e.what()});
    }
    LOG_INFO<<"\n------------------------------------------------------------------------------\n   "<<
        argv[0]<<" is done\n------------------------------------------------------------------------------\n";
    return 0;
}

int report_error(error_report&& err)
{
#if 0
    using namespace json::literals;
    json::output_stream root;
    auto parser = root ^ json::open;
    auto e = parser ^ "errors"_s;
    e ^ err ^ json::_end;
    auto msg = root ^ json::str_cast;
#endif
    const auto msg = to_json(err);
    std::cerr<<msg<<std::endl;
    return -1;
}

void report_success(const success_report& ok)
{
    using namespace json::literals;
    json::output_stream root;
    auto parser = root ^ json::open;
    auto err = parser ^ "ok"_s;
    err ^ ok ^ json::_end;
    std::cerr<<(root ^ json::str_cast)<<std::endl;
}


json::ostream& operator ^ (json::ostream& js, const success_report& ok)
{
    using namespace json::literals;

    js ^ "status"_n ^ ok.msg ^ "msg"_n ^ ok.details;
    return js;
}
dataclean_conf process_cmd(const std::string& arg)
{
    auto actions = process_configuration(arg);
    return actions;
}

void setup_log()
{
    static const std::string LOG_DIR_NAME = "logs";
    static const std::string LOG_FILE_NAME = "prepare4models";
    sl::default_log(log_severity, LOG_DIR_NAME.c_str(), LOG_FILE_NAME.c_str());
}

std::ostream& operator << (std::ostream& os, const schema_type& s) {
    std::copy(s.begin(), s.end(), std::ostream_iterator<schema_type::value_type>(os, "|"));
    return os;
}

void log_print_schema(const schema_type& s)
{
    std::string output = boost::algorithm::join(s, "|");
    LOG_DEBUG<<"shcema: "<<output;    
}

cleanup_results clean(const dataclean_conf& withconf)
{
    LOG_INFO<<"starting data cleaning process for data at "<<withconf.source_location;
    if (empty(withconf)) {
        LOG_ERROR<<"would not run, no valid configuration";
        return false;
    } else {
        cleanup_task cleanup;
        auto results = cleanup(withconf);
        if (results) {
            LOG_INFO<<"successfully finish cleaning up data and export it to '"<<withconf.export_file<<"'";
        } else {
            LOG_ERROR<<"failed to cleanup data";
        }
        LOG_INFO<<"finish processing all actions";
        return results;
    }
}
