#include "libs/json_parser/json_reader.h"
#include "libs/json_parser/jsonfwrd.h"

namespace json
{

bool read(std::istream& from, boost::property_tree::ptree& pt)
{
    try {
        boost::property_tree::read_json(from, pt);
        return true;
    } catch (std::exception& ) {

        return false;
    }
}


bool read(std::wistream& from, boost::property_tree::wptree& pt)
{
    try {
        boost::property_tree::read_json(from, pt);
        return true;
    } catch (std::exception& ) {
        return false;
    }
}

bool read(const std::string& input, boost::property_tree::ptree& pt)
{
    std::istringstream in(input);
    return read(in, pt);
}

bool read(const std::wstring& input, boost::property_tree::wptree& pt)
{
    std::wistringstream in(input);
    return read(in, pt);
}

}   // end of namespace json

