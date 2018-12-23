#include "logging.h"
#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/filesystem.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace sl
{

bool is_directory(const char* dir_path);
std::string log_file_full_name = "normal_messages.log";
std::string special_log_full_name = "important_messages.log";

std::ostream& operator<< (std::ostream& strm, severity_level level);
severity_level normal_log_level_setting = severity_level::disabled;
std::string log_message_tag_important_message = std::string("IMPORTANT_MESSAGE");

void set_log_severity(severity_level new_severity)
{
    if (new_severity < severity_level::disabled) {
        normal_log_level_setting = new_severity;
    } else {
        throw std::runtime_error{"invalid level - should not set to disable"};
    }
}

void disable_logging()
{
    normal_log_level_setting = severity_level::disabled;
}

std::ostream& operator<< (std::ostream& strm, severity_level level)
{
    static const char* strings[] =
    {
        "TRADCE",
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
        "CRITICAL",
        "NO LOG"
    };
    static constexpr auto size = sizeof(strings) / sizeof(*strings);
    if (static_cast< std::size_t >(level) < size) {
        strm << strings[level];
    } else {
        strm << "error: "<<static_cast< int >(level);
    }
    return strm;
}

void set_normal_log_file(const std::string& path)
{
    boost::filesystem::path p{path};
    if (is_directory(p.parent_path().c_str())) {
        log_file_full_name = path;
    } else {
        static const std::string message = "invalid path given to log directory ";
        throw std::runtime_error{message + path};
    }
}

void set_special_log_file(const std::string& path)
{
    boost::filesystem::path p{path};
    if (is_directory(p.parent_path().c_str())) {
        special_log_full_name = path;
    } else {
        static const std::string message = "invalid path given to log directory ";
        throw std::runtime_error{message + path};
    }
}

bool is_directory(const char* dir_path)
{
    boost::filesystem::path path{dir_path};
    if (!boost::filesystem::is_directory(path)) {
       return false;
    } else {
       return true;
    } 
}

void default_log(severity_level severity, 
        const char* log_dir, const char* logn)
{
    const std::string error_log_name = logn + std::string("errors.log");
    const std::string log_name{logn + std::string(".log")} ;

    auto cwd = boost::filesystem::current_path();
    boost::filesystem::path lp{cwd.parent_path()};

    lp /= log_dir;
    boost::filesystem::path elp{lp};
    lp /= log_name;
    elp /= error_log_name; 
    sl::set_normal_log_file(lp.string());
    sl::set_special_log_file(elp.string());
    sl::set_log_severity(severity);
}


}   // end of namespace sl

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", sl::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

struct my_handler
{
    typedef void result_type;

    void operator() (std::runtime_error const& e) const
    {
        std::cout << "std::runtime_error: " << e.what() << std::endl;
    }
    void operator() (std::logic_error const& e) const
    {
        std::cout << "std::logic_error: " << e.what() << std::endl;
        throw;
    }
};
bool filter_normal_log(const boost::log::attribute_value_set &set)
{
    auto i = set["Severity"].extract<sl::severity_level>(); 
    return i >= sl::normal_log_level_setting;
}

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt<sl::severity_level>) {
    src::severity_logger_mt<sl::severity_level> logger;
    logger.add_attribute("LineID", attrs::counter<unsigned int>(1));     // lines are sequentially numbered
    logger.add_attribute("TimeStamp", attrs::local_clock());             // each log line gets a timestamp
    // Setup the common formatter for all sinks
    logging::formatter fmt = expr::stream
        << std::setw(6) << std::setfill(' ') << line_id << std::setfill(' ')
        << expr::format_date_time(timestamp, "\t< [%Y-%m-%d] %H:%M:%S.%f >") << " "
        << ": <" << severity << ">\t"
        << expr::if_(expr::has_attr(tag_attr))
           [
               expr::stream << "[" << tag_attr << "] "
           ]
        << expr::smessage;

    // Initialize sinks
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >(sl::log_file_full_name));

    sink->set_filter(&filter_normal_log);
    sink->set_formatter(fmt);

    logging::core::get()->add_sink(sink);

    sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >(sl::special_log_full_name));

    sink->set_formatter(fmt);

    sink->set_filter(expr::attr< sl::severity_level >("Severity") >= sl::severity_level::warning || 
                     (expr::has_attr(tag_attr) && tag_attr == sl::log_message_tag_important_message));

    logging::core::get()->add_sink(sink);

    // Add attributes
    logging::add_common_attributes();
    return logger;
}


#if 0
void log_message(const char* msg, severity_level severity)
{
    src::severity_logger< severity_level > slg;
    BOOST_LOG_SEV(slg, severity) << msg;
}


void log_message(const char* msg, const char* tag)
{
    src::severity_logger< severity_level > slg;
    BOOST_LOG_SCOPED_THREAD_TAG(tag, msg);
}
#endif
