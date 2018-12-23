#include "apps/generate_model/process_input.h"
#include "apps/generate_model/execution_pipline.h"
#include "libs/system/exceptions.h"
#include "libs/system/error_message.h"
#include "logging.h"
#include <string>


int report_error(const std::string& msg, const std::string& d, bool log = true);
void setup_log(const char* name);

int main(int argc, char** argv)
{
    if (argc != 2) {
       return report_error("invalid args", "usage: <input json source (either from file or from CL)>", false);
    }
    try {
        setup_log("generate_model");
    } catch (...) {
        return report_error("log error", "failed to set log", false);
    }

    LOG_INFO<<"processing input message from front end:\n"<<argv[1];

    try {
        auto conf = process_input(argv[1]);
        if (pipeline::good(conf)) {
            LOG_INFO<<"successfully read configuration\n"<<conf;
        } else {
            LOG_ERROR<<"invalid configuration from front end\n"<<argv[1]<<"exiting";
            return report_error("invalid configuration", "the input configuration is invalid");
        }
        LOG_INFO<<"running model generation - trying to find optimized args and then saving the best model";
        const auto r = execute(conf);
        if (not r.empty()) {
            LOG_INFO<<"successfully finish finding the best model and saved it to "<<conf.output_path<<", lookup table\n"<<r;
            std::cout<<"\n"<<r<<std::endl;
            return 0;
        } else {
            LOG_ERROR<<"failed to generate model or to save it to "<<conf.output_path;
            return report_error("failed to generate", "failed to generate model or save it - unknown error");
        }
    } catch (const json_error& e) {
        return report_error("json error", e.what());
    } catch (const malformat_input& e) {
        return report_error("malformatted column", e.what());
    } catch (const invalid_conf& e) {
        return report_error("invalid configuration", e.what());
    } catch (const std::exception& e) {
        return report_error("critical error", e.what());
    } catch (...) {
        return report_error("unknown error", "faile to process, unknown error");
    }
    
}

int report_error(const std::string& msg, const std::string& d, bool log)
{
    error_report e{msg, d};
    if (log) {
        LOG_ERROR<<"error: "<<e;
    }
    const std::string m = to_json(e);
    std::cerr<<m<<std::endl;
    return -1;
}

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

