//#include "interfaces.h"
//#include "apps/model_prediction/schema_read.h"
#include "apps/model_prediction/processing_flow.h"
#include "libs/system/error_message.h"
#include "libs/system/exceptions.h"
#include "logging.h"
#include <iostream>
#include <algorithm>
#include <iterator>

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

int main(int argc, char** argv)
{
    if (argc != 2) {
       std::cerr<<to_json(error_report {"usage", std::string{"invalid params for "} +  argv[0]});
       return -1;
    }

    try {
        setup_log("model_prediction");
        const std::string cli{argv[1]};
        LOG_INFO<<"starting model prediction\nwith configuration\n----------------------------------------------\n"<<cli<<"----------------------------------------------\n";
        const auto output = read_scheme | cli | cli | predicator | formatter;
        LOG_INFO<<"the prediction results are\n-----------------------------------------\n"<<output<<"\n-----------------------------------------\n";
        std::cout<<output<<std::endl;
        LOG_INFO<<"successfully finish processing all rows from input source for model prediction";
        return 0;

    } catch (const invalid_input& e) {
        LOG_ERROR<<"input parameters error: "<<e.what();
        std::cerr<<to_json(error_report{"invalid input", "error with input: " + e.what()});
    } catch (const invalid_conf& e) {
        LOG_ERROR<<"invalid configuration: "<<e.what();
        std::cerr<<to_json(error_report{"invalid conf", "error with configuration: " + e.what()});
    } catch (const invalid_path& e) {
        LOG_ERROR<<"invalid path given: "<<e.what();
        std::cerr<<to_json(error_report{"invalid path", "error with path: " + e.what()});
    } catch (const json_error& e) {
        LOG_ERROR<<"json error: "<<e.what();
        std::cerr<<to_json(error_report{"json error", "error with json: " + e.what()});
    } catch (const std::exception& e) {
        LOG_ERROR<<"runtime error: "<<e.what();
        std::cerr<<to_json(error_report{"error", std::string{"error details: "} + e.what()});
    } catch (...) {
        LOG_ERROR<<"unknown error while processing - would abort";
        std::cerr<<to_json(error_report{"unknown error", "failed to procerss, unknown error"});
    }
        
    return -1;
}
