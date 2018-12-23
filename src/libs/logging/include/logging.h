#pragma once
#include <iosfwd>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

// We define our own severity levels
namespace sl
{
enum severity_level
{
    trace = 0,
    debug,
    info,
    warning,
    error,
    critical,
    disabled    // note that this level is used in order to make sure that we are not logging at all
};

void set_log_severity(sl::severity_level new_severity);
void set_normal_log_file(const std::string& path);
void set_special_log_file(const std::string& path);
void disable_logging();
void default_log(severity_level severity, const char* log_dir, const char* log_name);

}   // end of namespace sl

BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<sl::severity_level>)
//void init();
//void log_message(const char* msg, severity_level severity);
//void log_message(const char* msg, const char* tag);

// just a helper macro used by the macros below - don't use it in your code
#define INTERNAL_LOG_HELPER(severity) BOOST_LOG_SEV(logger::get(), severity)<<"["<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<"]"
// ===== log macros =====
#define LOG_TRACE   INTERNAL_LOG_HELPER(sl::severity_level::trace)
#define LOG_DEBUG   INTERNAL_LOG_HELPER(sl::severity_level::debug)
#define LOG_INFO    INTERNAL_LOG_HELPER(sl::severity_level::info)
#define LOG_WARNING INTERNAL_LOG_HELPER(sl::severity_level::warning)
#define LOG_ERROR   INTERNAL_LOG_HELPER(sl::severity_level::error)
#define LOG_FATAL   INTERNAL_LOG_HELPER(sl::critical)

