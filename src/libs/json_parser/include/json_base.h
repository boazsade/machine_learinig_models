#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace json
{

// this is when we have a value that is formatted already
// and we don't need to quote it like other strings
struct sub_tree {
	sub_tree(const std::string& s);

	const std::string& entry;
};

std::wstring widen_str(const std::string& str);
std::wstring widen_str(const char* str);

template<typename T>
struct entry
{
    typedef T value_type;

    explicit entry(const char* n, const T& val = T()) : name(n), value(val)
    {
    }

    void write(boost::property_tree::ptree& to) const
    {
        to.put<value_type>(name, value);
    }

    void write(boost::property_tree::wptree& to) const
    {
        to.put<value_type>(widen_str(name), value);
    }

    const char* name;
    value_type value;
};

template<>
struct entry<std::string>
{
    typedef std::string value_type;

    explicit entry(const char* n, const std::string& val = std::string()) : name(n), value(convert(val))
    {
    }

    void write(boost::property_tree::ptree& to) const
    { 
        to.put<std::string>(name, value);
    }

    const char* name;
    std::string value;

private:
    static std::string convert(const std::string& input)
    {
        std::string ret;
        ret.reserve(input.size() + 2);

		if (input.empty()) {
			ret = "\"\"";
		} else {           
            ret += '"';
            ret += input;
            ret += '"';
        }
        return ret;
    }
};

template<>
struct entry<sub_tree>
{
	typedef sub_tree value_type;

	explicit entry(const char* n, const value_type& v) : name(n), value(v)
	{

	}

	void write(boost::property_tree::ptree& to) const
	{
		to.put<std::string>(name, value.entry);
	}

	const char* name;
	value_type  value;
};

template<>
struct entry<std::wstring>
{
    typedef std::wstring value_type;

    explicit entry(const char* n, const std::wstring& val = std::wstring()) : name(n), value(convert(val))
    {
    }

    void write(boost::property_tree::wptree& to) const
    { 
        to.put<std::wstring>(widen_str(name), value);
    }

    const char* name;
    std::wstring value;

private:
    static std::wstring convert(const std::wstring& input)
    {
        std::wstring ret;
        ret.reserve(input.size() + 2);

		if (input.empty()) {
			ret = L"\"\"";
		} else {           
            ret += L'"';
            ret += input;
            ret += L'"';
        }
        return ret;
    }
};

template<typename T>
struct array_data : entry<T>
{
    typedef typename entry<T>::value_type value_type;

    explicit array_data(const value_type& v = value_type()) : entry<T>("", v)  // no names for arrays
    {
    }    
};

}   // end of namespace json
