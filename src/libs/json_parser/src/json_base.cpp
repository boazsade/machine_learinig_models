#include "libs/json_parser/json_base.h"

namespace json
{

std::wstring widen_str(const std::string& str)
{
    std::wostringstream wstm;

    const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());

    for(size_t i = 0; i < str.size(); ++i) {
        wstm<<ctfacet.widen(str[i]);
    }
    return wstm.str();
}

std::wstring widen_str(const char* str)
{
    size_t len = strlen(str);

    std::wostringstream wstm;

    const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());

    for(size_t i = 0; i < len; ++i) {
        wstm<<ctfacet.widen(str[i]);
    }
    return wstm.str();
}

}   // end of namespace json
