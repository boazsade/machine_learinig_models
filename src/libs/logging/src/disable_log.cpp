#include "ut_details/disable_log.h"
#include "logging.h"
struct disable_log
{
    disable_log() {
        sl::set_normal_log_file("/dev/null");
        sl::set_special_log_file("/dev/null");
        sl::disable_logging();
    }
};

void do_disable_log()
{
    static const disable_log rr = disable_log{}; 
    (void)rr;
}

