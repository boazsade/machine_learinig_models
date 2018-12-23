#include <string>
#include <iosfwd>

struct error_report {
    const std::string msg;
    const std::string details;

    error_report(const std::string& m, const std::string& d) :
        msg(m), details(d)
    {
    }
};

std::ostream& operator << (std::ostream& os, const error_report& er);
std::string to_json(const error_report& er);


