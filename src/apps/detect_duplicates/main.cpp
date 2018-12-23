#include "apps/detect_duplicates/duplication_conf.h"
#include "apps/detect_duplicates/actions.h"
#include "apps/detect_duplicates/duplication_action.h"
#include "apps/detect_duplicates/duplication_task.h"
#include "apps/detect_duplicates/command_processing.h"
#include "apps/detect_duplicates/outliers_task.h"
#include "apps/detect_duplicates/outliers_action.h"
#include "apps/detect_duplicates/outliers_conf.h"
#include "apps/detect_duplicates/exceptions.h"
#include "libs/sources_handling/input_data.h"
#include "libs/json_parser/json_ostream.h"
#include "libs/json_parser/json_writer.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"
#include "logging.h"
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <iterator>

const std::string err_msg_begin = "{\"error\": {\"msg\":";
const std::string err_msg_end = "}}";

struct detailed_stats {
    detailed_stats(const row_numbers& d, const row_numbers& o,
            const row_numbers& m) :
        dup(d), outliers(o), missing(m)
    {
    }

    const row_numbers& dup;
    const row_numbers& outliers;
    const row_numbers& missing;
};

struct data_stats {
    constexpr data_stats() = default;
    constexpr data_stats(std::size_t dc, std::size_t oc, std::size_t mc) :
        dup_count(dc), ol_count(oc), miss_count(mc)
    {
    }

    std::size_t dup_count = 0u;
    std::size_t ol_count = 0u;
    std::size_t miss_count = 0u;
};

#ifdef NDEBUG
    sl::severity_level log_severity = sl::info;
#else 
    sl::severity_level log_severity = sl::debug;
#endif // NDEBUG
struct error_report {
    const std::string msg;
    const std::string details;

    error_report(const std::string& m, const std::string& d) :
        msg(m), details(d)
    {
    }
};

std::ostream& operator << (std::ostream& os, const error_report& err);
//std::ostream& operator << (std::ostream& os, const app_args& args);
json::ostream& operator ^ (json::ostream& js, const dupliations_report& dr);
json::ostream& operator ^ (json::ostream& js, const outlier_report& olr);
json::ostream& operator ^ (json::ostream& js, const detailed_stats& stats);
json::ostream& operator ^ (json::ostream& os, const input_data& dr);
json::ostream& operator ^ (json::ostream& js, const error_report& err);
json::ostream& operator ^ (json::ostream& js, const data_stats& err);

void setup_log();
void report_errors(const error_report& errors);
int parse_cli(int argc, char** argv, app_args& ac);
void report_finding(const finiding_summery& report);

void report_errors(const error_report& errors)
{
    using namespace json::literals;
    json::output_stream root;
    auto parser = root ^ json::open;
    auto err = parser ^ "errors"_s;
    err ^ errors ^ json::_end;
    auto msg = root ^ json::str_cast;
    std::cerr<<msg<<std::endl;
}

void report_finding(const finiding_summery& info)
{
    json::output_stream root;
    auto parser = root ^ json::open;
    parser ^ info ^ json::_end;
    auto msg = root ^ json::str_cast;
    std::cout<<msg<<std::endl;
}

json::ostream& operator ^ (json::ostream& js, const detailed_stats& stats)
{
    using namespace json::literals;
    js ^ "duplication"_s ^ stats.dup ^ json::_end;
    js ^ "outliers"_s ^ stats.outliers ^ json::_end;
    js ^ "missings"_s ^ stats.missing ^ json::_end;
    return js;
}

json::ostream& operator ^ (json::ostream& js, const data_stats& stats)
{
    using namespace json::literals;
    js ^ "duplications"_n ^ stats.dup_count ^ "outliers"_n ^ stats.ol_count ^ "missings"_n ^ stats.miss_count;
    return js;
}

std::ostream& operator << (std::ostream& os, const error_report& err) {
    return os<<"error code: "<<err.msg<<"\ndetails: "<<err.details;
}

json::ostream& operator ^ (json::ostream& js, const error_report& err) {
    using namespace json::literals;

    js ^ "error"_n ^ err.msg ^ "msg"_n ^ err.details;
    return js;
}

json::ostream& operator ^ (json::ostream& os, const input_data& dr)
{
    using namespace json::literals;
    os ^ "row"_n ^ dr.row ^ "value"_n ^ dr.value;
    return os;
}

json::ostream& operator ^ (json::ostream& js, const dupliations_report& dr)
{
    using namespace json::literals;
    js ^ "dup.count"_n ^ dr.dups.size() ^  "column"_n ^ dr.column ^ "file"_n ^ dr.path ^ "rows"_n ^ dr.total;
    auto d = js ^ "details"_s;
    d ^ dr.dups ^ json::_end;
    return js;
}

json::ostream& operator ^ (json::ostream& js, const outlier_report& olr)
{
    using namespace json::literals;
    js ^ "column"_n ^ olr.column ^ "file"_n ^ olr.path ^ "rows"_n ^ olr.total;
    auto s = js ^ "stats"_s;
    //s ^ "dup.count"_n ^ olr.dups.size() ^ "outliers.count"_n ^ olr.outliers.size() ^ json::_end;
    data_stats st{olr.dups.size(), olr.outliers.size(), olr.missings.size()};
    s ^ st ^ json::_end;
    auto d = js ^ "details"_s;
    detailed_stats ds{olr.dups, olr.outliers, olr.missings};
    d ^ ds ^ json::_end;
    return js;

}


void setup_log()
{
    static const std::string LOG_DIR_NAME = "logs";
    static const std::string LOG_FILE_NAME = "duplications_outliers";
    sl::default_log(log_severity, LOG_DIR_NAME.c_str(), LOG_FILE_NAME.c_str());
}

int main(int argc, char** argv)
{
    try {
        setup_log();
    } catch (const std::exception& e) {
        report_errors(error_report{"failed to setup log", e.what()});
        return -2;
    }
    if (argc != 2) {
        error_report err{"usage missing args", "must ve called with input  json file"};
        report_errors(err);
        LOG_ERROR<<"program called without args or with invalid number of args";
        return -1;
    }

    app_args info;
    try {
        LOG_INFO<<"\n-----------------------------------------------\n\tstarting duplication finiding\nargs:\n"<<argv[1]<<"\n-----------------------------------------------\n";
        if (!parse_command(argv[1], info)) {
            LOG_ERROR<<"invalid command input: "<<((info.duplications.empty() && info.outliers.empty()) ? "no valid actions" : "invalid path for input");
            if (info.duplications.empty()) {
                error_report err{"no valid duplication actions", "no valid duplication filter actions were passed"};
                report_errors(err);
            } else if (info.outliers.empty()) {
                error_report err{"no valid outliers actions", "no valid outlier find actions were passed"};
                report_errors(err);
            } else {
                LOG_ERROR<<"unknown error while parsing command input";
                error_report err{"unknown error", "failed to process input command - uknown error"};
                report_errors(err);
            }
            return -5;
        } else {
            if (!boost::filesystem::exists(info.input_path)) {
                error_report err{"invalid path", std::string{"the path '"} + info.input_path.string() + "' do not exists"};
                report_errors(err);
                return -4;
            }
            auto res = find_duplications(info);
            auto report = find_outliers(info, res);
            report_finding(report);
            LOG_INFO<<"\n-----------------------------------------------\n\tfinish duplication finiding\n-----------------------------------------------\n";
        }
    } catch (const std::runtime_error& re) {
        error_report err{"run time error", re.what()};
        report_errors(err);
        LOG_ERROR<<"run time error: "<<re.what();
        return -2;
    } catch (const no_such_path& nsf) {
        error_report err{"no such file", nsf.what()};
        report_errors(err);
        LOG_ERROR<<"no such file as input to dupliation processing: "<<nsf.what();
        return -3;
    } catch (const access_denied& ad) {
        error_report err{"access denied", ad.what()};
        report_errors(err);
        LOG_ERROR<<"cannot process input file, access defined "<<ad.what();
        return -5;
    } catch (const configuration_error& ce) {
        report_errors(error_report{"invalid configuration", ce.what()});
        LOG_ERROR<<"configuration error: "<<ce.what();
        return -7;
    } catch (const std::exception& exp) {
        error_report err{"dupliation decection error", exp.what() };
        report_errors(err);
        LOG_ERROR<<"error while trying to find duplications and outliers: "<<exp.what();
        return -6;
    }
    return 0;
}
