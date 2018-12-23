#pragma once

#include "json_base.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/mpl/if.hpp> // boost::mpl::if_c
#include <boost/type_traits/is_same.hpp>
#include <sstream>
#include <iostream>
#include <boost/foreach.hpp>

namespace json
{

namespace details
{

template<typename T>
struct null_string;

template<>
struct null_string<char>
{
    static const char* get()
    {
        return "";
    }
};

template<>
struct null_string<wchar_t>
{
    static const wchar_t* get()
    {
        return L"";
    }
};

template<typename T>
struct special_chars;

template<>
struct special_chars<char>
{
    static char open_square()
    {
        return '[';
    }

    static char closing_square()
    {
        return ']';
    }

    static char dots()
    {
        return ':';
    }
};

template<>
struct special_chars<wchar_t>
{
    static wchar_t open_square()
    {
        return L'[';
    }

    static wchar_t closing_square()
    {
        return L']';
    }

    static wchar_t dots()
    {
        return L':';
    }
};

}   // end of namespace details

template<typename T>
struct array_reader
{
	T operator () (const char* name, boost::property_tree::ptree::value_type& entry) const
	{
		return entry.second.get<T>(name);
	}

	T operator () (const wchar_t* name, boost::property_tree::wptree::value_type& entry) const
	{
		return entry.second.get<T>(name);
	}
};

template<typename T>
struct reader
{
	T operator () (const char* name, boost::property_tree::ptree& pt) const
	{
		return pt.get<T>(name);
	}

	T operator () (const wchar_t* name, boost::property_tree::wptree& pt) const
	{
		return pt.get<T>(name);
	}
};

template<typename T, typename CharT = char>
struct single_entry : reader<T>
{
	typedef T	                         value_type;
    typedef CharT                        char_type;
    typedef std::basic_string<char_type> string_type;

    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;
	
	single_entry(const char_type* n, const T& default_val = T()) : name(n), value(default_val)
	{
	}

	bool read(proptree_type& pt)
	{
		try {
			value = this->operator()(name.c_str(), pt);
			return true;
		} catch (std::exception& e) {
			return false;
		}
	}

	string_type name;
	value_type  value;
};

template<typename T, typename CharT = char>
struct ref_single_entry : reader<T>
{
	typedef T	                         value_type;
    typedef CharT                        char_type;
    typedef std::basic_string<char_type> string_type;

    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;
	
	ref_single_entry(const char_type* n, T& default_val) : name(n), value(default_val)
	{
	}

	bool read(proptree_type& pt)
	{
		try {
			value = this->operator()(name.c_str(), pt);
			return true;
		} catch (std::exception& e) {
			return false;
		}
	}

	string_type name;
	value_type&  value;
};

template<typename T, typename CharT> inline
std::basic_ostream<CharT>& operator << (std::basic_ostream<CharT>& os, const single_entry<T, CharT>& entry)
{
	return os<<details::special_chars<CharT>::open_square()<<entry.name<<details::special_chars<CharT>::dots()<<entry.value<<details::special_chars<CharT>::closing_square();
}

template<typename T, typename CharT = char>
struct array_entry : array_reader<T>
{
	typedef T	value_type;
    typedef CharT char_type;
    typedef std::basic_string<char_type> string_type;
    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;
	
	array_entry(const char_type* n, const T& default_val = T()) : name(n), value(default_val)
	{
	}

	bool read(typename proptree_type::value_type& pt)
	{
		try {
			value = this->operator()(name.c_str(), pt);
			return true;
		} catch (std::exception&) {
			return false;
		}
	}

	string_type name;
	value_type value;
};

template<typename T, typename CharT = char>
struct ref_array_entry : array_reader<T>
{
	typedef T	value_type;
    typedef CharT char_type;
    typedef std::basic_string<char_type> string_type;
    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;
	
	ref_array_entry(const char_type* n, T& default_val) : name(n), value(default_val)
	{
	}

	bool read(typename proptree_type::value_type& pt)
	{
		try {
			value = this->operator()(name.c_str(), pt);
			return true;
		} catch (std::exception&) {
			return false;
		}
	}

	string_type name;
	value_type& value;
};

template<typename T, typename CharT> inline
std::basic_ostream<CharT>& operator << (std::basic_ostream<CharT>& os, const array_entry<T, CharT>& entry)
{
	return os<<details::special_chars<CharT>::open_square()<<entry.name<<details::special_chars<CharT>::dots()<<entry.value<<details::special_chars<CharT>::closing_square();
}

typedef array_entry<int>		    int_array_entry;
typedef array_entry<std::string>	str_array_entry;
typedef array_entry<double>		    fp_array_entry;
typedef array_entry<bool>		    bool_array_entry;
typedef single_entry<int>		    int_entry;
typedef single_entry<std::string>	str_entry;
typedef single_entry<double>		fp_entry;
typedef single_entry<bool>		    bool_entry;
////////////////////
// wide version
typedef array_entry<int, wchar_t>		    wint_array_entry;
typedef array_entry<std::wstring, wchar_t>	wstr_array_entry;
typedef array_entry<double, wchar_t>		wfp_array_entry;
typedef array_entry<bool, wchar_t>		    wbool_array_entry;
typedef single_entry<int, wchar_t>		    wint_entry;
typedef single_entry<std::wstring, wchar_t>	wstr_entry;
typedef single_entry<double, wchar_t>		wfp_entry;
typedef single_entry<bool, wchar_t>		    wbool_entry;

template<typename T, typename CharT = char>
struct values_list
{
private:
    typedef std::vector<T>  data_type;
    typedef CharT char_type;
    typedef std::basic_string<char_type> string_type;
    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;

public:

    typedef typename data_type::const_iterator const_iterator;

    typedef T   value_type;

    explicit values_list(const string_type& input = string_type(), const char_type* root = details::null_string<char_type>::get())
    {
        if (!input.empty()) {
            read(input, root);
        }
    }

    bool read(const string_type& input, const char_type* root = details::null_string<char_type>::get())
    {
        data.clear();

        proptree_type pt;

        if (!input.empty()) {
            if (this->read(input, pt)) {
                BOOST_FOREACH(typename proptree_type::value_type &v, pt.get_child(root)) {
                    array_entry<value_type, char_type> elem(details::null_string<char_type>::get());
                    bool ret = elem.read(v);
                    if (ret) {
                        data.push_back(elem.value);
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    const T& operator [] (std::size_t index) const
    {
        return data.at(index);
    }

    const_iterator begin() const
    {
        return data.begin();
    }

    const_iterator end() const
    {
        return data.end();
    }

    bool empty() const
    {
        return data.empty();
    }

    std::size_t size() const
    {
        return data.size();
    }

private:
    bool read(const std::string& , proptree_type& pt)
    {
        std::basic_istringstream<char_type> buffer;
        try {
            read_json(buffer, pt);
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }

private:
    data_type   data;
};

// read json into property tree
bool read(std::istream& from, boost::property_tree::ptree& pt);
bool read(std::wistream& from, boost::property_tree::wptree& pt);
bool read(const std::string& input, boost::property_tree::ptree& pt);
bool read(const std::wstring& input, boost::property_tree::wptree& pt);

}   // end of namespace json
