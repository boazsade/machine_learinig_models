#include "logging.h"
#include "apps/csv_info/csv_info.h"
#include "apps/csv_info/errors_types.h"
#include "libs/json_parser/json_ostream.h"
#include "libs/json_parser/json_writer.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"
#include "utils/algorithms.hpp"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include  <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

const std::string err_msg_begin = "{\"error\": {\"msg\":";
const std::string err_msg_end = "}}";

#ifdef NDEBUG
    static const sl::severity_level log_severity = sl::info;
#else 
    static const sl::severity_level log_severity = sl::debug;
#endif // NDEBUG

void setup_log()
{
    static const std::string LOG_DIR_NAME = "logs";
    static const std::string LOG_FILE_NAME = "csv_info";
    sl::default_log(log_severity, LOG_DIR_NAME.c_str(), LOG_FILE_NAME.c_str());
}


struct error_report {
    const std::string msg;
    const std::string details;
    const std::string file;

    error_report(const std::string& m, const std::string& d, const std::string& f) :
        msg(m), details(d), file(f)
    {
    }
};

std::ostream& operator << (std::ostream& os, const error_report& err) {
    return os<<"error code: "<<err.msg<<"\ndetails: "<<err.details;
}

json::ostream& operator ^ (json::ostream& js, const error_report& err) {
    using namespace json::literals;

    js ^ "error"_n ^ err.msg ^ "msg"_n ^ err.details ^ "file"_n ^ err.file;
    return js;
}

json::ostream& operator ^ (json::ostream& js, const csv_info::entry_type& entry) {
    using namespace json::literals;

    js ^ "index"_n ^ entry.index ^ "type"_n ^ guess_name(entry) ^ "count"_n ^ entry.total ^
        "missing"_n ^ entry.missing;
    auto stats = js ^ "stats"_s;
    stats ^ "strings"_n ^ entry.str_count.count() ^ "numerics"_n ^ entry.num_count.count() ^ "dates"_n ^ entry.date_count.count() ^ json::_end;
    auto samples = js ^ "samples"_s;
    samples ^ entry.samples() ^ json::_end;
    return js;
}

json::ostream& operator ^ (json::ostream& js, const csv_info& info) {
    using namespace json::literals;
    
    auto sub = js ^ "cols"_s;
    sub ^ info.all() ^ json::_end;
    return js;
};

struct app_args {
    char delim = ',';
    char quote = '"';
    std::size_t headers_count = 0u;
    std::size_t others_headers_count = 0u;

    std::vector<std::string> files;

    app_args() = default;
};

std::ostream& operator << (std::ostream& os, const app_args& aa)
{
    os<<"delimiter: ["<<aa.delim<<"], quated text: ["<<aa.quote<<"], headers line count: "<<aa.headers_count
        <<", other files headers count: "<<aa.others_headers_count<<"\ninput files: [";
    return out_range(os, aa.files, ", ")<<"]";
}

json::istream& operator ^ (json::istream& in, app_args& arg) {
    using namespace json::literals;

    in ^ "delimiter"_n ^ arg.delim ^ "quote"_n ^ arg.quote ^ "firstRow"_n ^ arg.headers_count ^ "otherRow"_n ^ arg.others_headers_count;
    if (in) {
        auto er = in ^ json::_child(in, "inputFiles"_n); 
        er ^ json::start_arr ^ arg.files ^ json::end_arr;
    } else {
        throw std::runtime_error{"failed to parse args from json input"};
    }
    return in;
}

using errors_list = std::vector<error_report>;

const std::string no_such_file_error = "no_such_file";
const std::string empty_file_error = "empty_file";
const std::string invalid_columns_count_error = "invalid_columns_count";
const std::string parsing_error = "csv_parsing_error";
const std::string other_error = "unknown_error";

void process_file(const std::string& file, csv_info& to, char delim, 
        char quote, std::size_t skip_first);
void report_errors(const errors_list& errors);
void report_finding(const csv_info& info);
int parse_cli(int argc, char** argv, app_args& ac);
int process_json_input(app_args& ac, const std::string& path);

void process_file(const std::string& file, csv_info& to, char delim, char quote, 
        std::size_t skip_first)
{
    std::ifstream input(file);
    if (!input) {
        static const auto err = no_such_file{};
        throw err;
    }
    csv_info::process(input, to, delim, quote, skip_first);
    LOG_INFO<<"successfully processed file "<<file<<"\n"<<to<<std::endl;
}

void report_errors(const errors_list& errors)
{
    using namespace json::literals;
    json::output_stream root;
    auto parser = root ^ json::open;
    auto err = parser ^ "errors"_s;
    err ^ errors ^ json::_end;
    auto msg = root ^ json::str_cast;
    std::cerr<<msg<<std::endl;
}

void report_finding(const csv_info& info)
{
    json::output_stream root;
    auto parser = root ^ json::open;
    parser ^ info ^ json::_end;
    auto msg = root ^ json::str_cast;
    std::cout<<msg<<std::endl;
}

int process_json_input(app_args& ac, const std::string& path)
{
    std::ifstream input{path};
    if (!input) {
        std::cerr<<"failed to open "<<path<<" configurtion file for reading\n";
        return -1;
    } else {
        try {
            json::istream_root root; 
            try {
                root ^ input;
                auto base_node = root ^ json::_root; 
                auto conf = base_node ^ json::_child(base_node, json::_name("conf")); 
                conf ^ ac; 
            } catch (...) {
                std::cerr<<"failed to parser json input file\n";
                return -1;
            }
            if (ac.files.empty()) {
                std::cerr<<"no input files found in "<<path<<std::endl;
                return -1;
            } else {
                //std::copy(std::begin(ac.files), std::end(ac.files), std::ostream_iterator<std::string>(std::cout, "\n"));
                //std::cout<<"delimiter: ["<<ac.delim<<"], quote: ["<<ac.quote<<"]\n";
                return 1; 
            }
        } catch (...) {
            std::cerr<<"failed to parse configuration file "<<path<<std::endl;
            return -1;
        }
    }
}

namespace po = boost::program_options;
int parse_cli(int argc, char** argv, app_args& ac)
{
    
    std::string json_conf, char_set = "iso8601";
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("quote,q", po::value<char>(&ac.quote)->default_value(ac.quote), 
                "CSV record quoation char")
            ("delimiter,d", po::value<char>(&ac.delim)->default_value(ac.delim), 
                   "CSV delimiter char")
            ("json-conf,j", po::value<std::string>(&json_conf),
             "JSON configuration file (note that this option is exclusive with other options")
            ("first-row,r", po::value<std::size_t>(&ac.headers_count)->default_value(ac.headers_count),
             "number of headers lines in first input file")
            ("other-row,o", po::value<std::size_t>(&ac.others_headers_count)->default_value(ac.others_headers_count),
             "number of headers lines in all files other then the first input file")
            ("char-set,c", po::value<std::string>(&char_set)->default_value(char_set),
             "the char set at which the CSV files are encoded in")
            ("input-files,f", po::value< std::vector<std::string> >(), "input file");
    po::positional_options_description p;
    p.add("input-files", -1);

    po::variables_map cli;
    if (argc == 1 || cli.count("help")) {
        std::cerr<<desc<<std::endl;
        return 0;
    }
    try {
        po::store(po::command_line_parser(argc, argv).
                      options(desc).positional(p).run(), cli);
        po::notify(cli);
    } catch (const std::exception& e) {
        std::cerr<<"error - invalid command line: "<<e.what()<<"\n"<<desc<<std::endl;
        return -1;
    }
    if (cli.count("json-conf")) {
        return process_json_input(ac, json_conf);
    } else {
        if (cli.count("input-files")) {
            ac.files = cli["input-files"].as<std::vector<std::string>>();
        } else {
            std::cerr<<"error - missing command line args for input files!\n"<<desc<<std::endl;
            return -1;
        }
    }
    return 1;
}

int main(int argc, char** argv)
{
    app_args args;
    auto ret = parse_cli(argc, argv, args);
    if (ret == 0) {
        return 0;
    } else {
        if (ret < 0) {
            std::cerr<<"invalid command line args\nexiting\n";
            return -1;
        }
    }
    errors_list errors;
    try {
        setup_log();
    } catch (const std::exception& e) {
        errors.push_back(errors_list::value_type{"fail to start", e.what(), ""});
        report_errors(errors);
        return -1;
    }
    csv_info info;
    LOG_WARNING<<"\n=========================\n=========================\nstarting csv info application\nusing configuration\n"
                <<args<<"\n=========================\n=========================\n";
    auto header_skips = args.headers_count;
    for (auto i = args.files.begin(); i != args.files.end(); ++i) {
        try {
            LOG_INFO<<"processing into file "<<*i;
            process_file(*i, info, args.delim, args.quote, header_skips);
            header_skips = args.others_headers_count;
        } catch (no_such_file) {
            static const std::string d{"file not found"};
            LOG_ERROR<<"error: failed not found "<<*i;
            errors.emplace_back(no_such_file_error, d, *i);
        } catch (empty_input_file) {
            LOG_ERROR<<"error: empty file "<<*i;
            static const std::string d{"empty file found"};
            errors.emplace_back(empty_file_error, d, *i);
        } catch (const invalid_columns_count& err) {
            const std::string d{"expected number of columns: " + std::to_string(err.expected) +
                " do not match number of columns, found: " + std::to_string(err.actual)};
            errors.emplace_back(invalid_columns_count_error, d, *i);
            LOG_ERROR<<"error in file "<<*i<<": "<<d;
        } catch (csv_parsing_error) {
            static const std::string d{"parsing error of csv"};
            LOG_ERROR<<"error in file "<<*i<<" parsing error";
            errors.emplace_back(parsing_error, d, *i);
        } catch (const std::runtime_error& e) {
            const std::string d{"error found while processing "};
            std::string er = d + ", error: " + e.what();
            LOG_ERROR<<"error while processing file "<<*i<<": "<<er;
            errors.emplace_back(other_error, er, *i);
        }
    }

    if (errors.empty()) {
        LOG_INFO<<"successfully finish processing";
        report_finding(info);
        return 0;
    } else {
        LOG_WARNING<<"finish processing - with errors";
        report_errors(errors);
        return -2;
    }
}
