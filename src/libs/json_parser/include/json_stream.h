#pragma once
#include <boost/mpl/if.hpp> // boost::mpl::if_c
#include <boost/type_traits/is_same.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp> 
#include <boost/type_traits/is_function.hpp>

namespace json
{

namespace details {
    template<typename T>
    struct check_legal_value {
        static const bool value =  !(boost::is_member_function_pointer<T>::value ||
                                   boost::is_pointer<T>::value ||
                                   boost::is_function<T>::value
                                );
    };
}   // end of namespace details

template<typename Ch>
struct ptree_type
{
    typedef Ch char_type;
    typedef typename boost::mpl::if_c<boost::is_same<char_type, char>::value,
                                 boost::property_tree::ptree,
                                 boost::property_tree::wptree>::type proptree_type;

};

// placehold so that we would using array as insertion and not
// using name, value pair into the data
//static const struct __array
//{
//} _array = __array();
//
// to allow overloading for exterction of data based on name
struct _name
{
    _name(const char* n) : value(n)
    {
    }

    _name(const char* n, std::size_t): value(n)
    {
    }

    const char* value;
};

inline namespace literals
{

inline _name operator "" _n(const char* s)
{
    return _name(s);
}

inline _name operator "" _n(const char* s, std::size_t sz)
{
    return _name(s, sz);
}

}   // end of namespace literals

struct json_stream
{    
    json_stream() : ok(true), op_val(false), name(0)
    {
    }

    explicit json_stream(bool stat, 
            bool op = false, const char* n = nullptr) :
                    ok{stat}, op_val{op}, name{n}
    {
    }

    bool good() const
    {
        return ok;
    }

    bool bad() const
    {
        return !good();
    }

    typedef bool(json_stream::*boolean_type)() const;

    operator boolean_type () const
    {
        return good() ? &json_stream::good : (boolean_type)0;
    }

    const char* element_name() const
    {
        return name;
    }

    void set(const _name& v)
    {
        if (good()) {
            name = v.value;
        }
    }

    void reset()
    {
        name = nullptr;
        op_val = false;
    }

    void set_state(bool val) 
    {
        ok = val;
    }

    void set_op(bool yes)
    {
        op_val = yes;
    }

    bool is_op() const
    {
        return op_val;
    }

private:
    bool ok = true;
    bool op_val = false;
    const char* name = nullptr;
};

// use to to signal that this variable that we are 
// reading is optional in the json message
static const struct optional_
{
}   optional = optional_();

}   // end of namespace json

