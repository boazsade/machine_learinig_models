#pragma once
#include "json_stream.h"
#include "json_reader.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/static_assert.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <list>
#include <set>
#include <fstream>

namespace json
{

template<typename Ch>
struct basic_istream;

template<typename T>
struct __child
{
    __child(/*const */basic_istream<T>& is, const _name& ne) : stream(is), n(ne)
    {
    }

    basic_istream<T> get();

private:
    /*const*/ basic_istream<T>& stream;
    _name            n;
};

struct __array_start {};
struct __array_end{};

constexpr __array_start start_arr = __array_start{};
constexpr __array_end end_arr = __array_end{};

static const struct __Container {} _container = __Container(); 

template<typename Ch>
class basic_istream_root;
// this class is the class we are using in order to construct 
// JSON message - in this case we are passing to the it
// so data that we want to extract and it would extract it from
// the JSON message. For examle if we are on the JSON "tree" 
// and we have an entry with the name "foo" and the value is
// an integer, we would extract it with istream_obj ^ "foo"_n ^ my_int;
// note that this is not a class that you can costruct directly
template<typename Ch>
struct basic_istream : json_stream
{
    typedef typename ptree_type<Ch>::proptree_type  proptree_type;
    typedef typename ptree_type<Ch>::char_type      char_type;

    friend class basic_istream_root<Ch>;

    basic_istream(proptree_type& p) : pt(p)
    {
    }
    basic_istream(proptree_type& p, bool stat) : json_stream{stat}, pt(p)
    {
    }

    basic_istream() = delete;
    basic_istream(const basic_istream&) = default;
    basic_istream(basic_istream&&) = default;
    basic_istream& operator = (const basic_istream&) = default;
    basic_istream& operator = (basic_istream&&) = default;
    // are are supporting only 4 basic types:
    // string, int, boolean and double
    // and the derived types from these:
    // all unsigned and float
    basic_istream& operator ^ (bool & val)
    {
        return this->extract<bool>(val);
    }

    basic_istream& operator ^ (int & val)
    {
        return this->extract<int>(val);
    }

    basic_istream& operator ^ (long & val)
    {
        return this->extract<long>(val);
    }

    basic_istream& operator ^ (unsigned long & val)
    {
        return this->extract<unsigned long>(val);
    }

    basic_istream& operator ^ (long long & val)
    {
        return this->extract<long long>(val);
    }

    basic_istream& operator ^ (unsigned long long & val)
    {
        return this->extract<unsigned long long>(val);
    }

    basic_istream& operator ^ (double & val)
    {
        return this->extract<double>(val);
    }

    basic_istream& operator ^ (std::string & val)
    {
        return this->extract<std::string>(val);
    }

    // add support for some build in types that 
    // we can support as well (note that this requirs cast)
    basic_istream& operator ^ (short & val)
    {
        return this->extract<short>(val);
    }

    basic_istream& operator ^ (unsigned short & val)
    {
        return this->extract<unsigned short>(val);
    }

    basic_istream& operator ^ (unsigned int & val)
    {
        return this->extract<unsigned int>(val);
    }

    basic_istream& operator ^ (unsigned char & val)
    {
        return this->extract<unsigned char>(val);
    }

    basic_istream& operator ^ (char & val)
    {
        return this->extract<char>(val);
    }

    basic_istream& operator ^ (float & val)
    {
        return this->extract<float>(val);
    }

    // overload for array - start of array, that means we don't have key only value
    // and for the end of it
    basic_istream& operator ^ (__array_start)
    {
        array_entries = true; 
        return *this;
    }

    basic_istream& operator ^ (__array_end)
    {
        array_entries = false;
        return *this;
    }

    // overload for name
    basic_istream& operator ^ (const _name& v)
    {
        this->set(v);
        return *this;
    }

    // return a sub element in the tree
    basic_istream operator ^ (/*const*/ __child<Ch> c)
    {
        return c.get();
    }

    // read optional value from the input
    basic_istream& operator ^ (const optional_&)
    {
        this->set_op(true);
        return *this;
    }

    //basic_istream operator ^ (const __child<Ch>& c)
    // this function is called to desend into the tree
    // i.e. to instruct that we are going into the tree
    // so that the name would not be used to extract 
    // element but a sub tree
    basic_istream operator () (const _name& v) const
    {
        return get_child(v);
    }

    basic_istream get_child(const _name& v) const
    {
        return basic_istream(pt.get_child(v.value));
    }

    proptree_type& entries()
    {
        return pt;
    }

    const proptree_type& entries() const
    {
        return pt;
    }

    basic_istream& operator ^ (const __Container& )
    {
        return *this;
    }

private:

    template<typename T>
    basic_istream& extract(T& val)
    {
        return /*this->array_entries ?*/ this->extract_(val) /*: this->extract_elem(val, pt.front())*/;
    }
    
    template<typename T>
    basic_istream& extract_(T& val)
    {
        BOOST_STATIC_ASSERT(details::check_legal_value<T>::value);
        if (this->good() && this->element_name()) {
            ref_single_entry<T> i(this->element_name(), val);
            bool st = i.read(pt);
            if (!this->is_op()) {   // only if this should be mandatory value, if not then ignore fail to read
                this->set_state(st);
            }
        } 
        this->reset(); 
        return *this;
    }

    template<typename T>
    basic_istream& extract_elem(T& val, typename proptree_type::value_type& entry)
    {
        if (this->good()) {
            ref_array_entry<T> i("", val);
            bool st = i.read(entry);

            if (!this->is_op()) {   // only if this should be mandatory value, if not then ignore fail to read
                this->set_state(st);
            }
        } 
    
        return *this;
    }

private:
    proptree_type& pt;
    bool           array_entries = false;
};

struct __root {};
constexpr __root _root = __root{};

// this class would be used to initaite the istream for json
// you cannot directly created json's istream, but this class
// can. Once you start the istream with this class you can
// start parsing and extract data from the JSON stream
template<typename Ch>
class basic_istream_root
{
public:

    using stream_type = basic_istream<Ch>;
    using proptree_type = typename stream_type::proptree_type;
    using boolean_type = bool(basic_istream_root<Ch>::*)()const;

    basic_istream_root() : state{false}, tree_root{}
    {
    }

    basic_istream_root(const std::string& input) : state{false}
    {
        if (!open(input)) {
            throw std::runtime_error{"failed to read JSON from " + input};
        }
    }

    basic_istream_root(std::istream& source)  : state{false}
    {
        if (!open(source)) {
            throw std::runtime_error{"failed to read JSON input from stream source"};
        }
    }

    // to open a path and read from a file - you must used boost's filesytem (soon to be part of standard)
    basic_istream_root(const boost::filesystem::path& file_path) : state{false}
    {
        if (!open(file_path)) {
            throw std::runtime_error{"failed to read JSON input from " + file_path.string()};
        }
    }
    

    bool open(const std::string& input)
    {
        state = read(input, tree_root);
        return state;
    }

    bool open(std::istream& source)
    {
        state = read(source, tree_root);
        return state;
    }

    bool open(const boost::filesystem::path& file_path)
    {
        std::basic_ifstream<Ch> read_open(file_path.string());
        if (read_open) {
            return open(read_open);
        } else {
            state = false;
            return state;
        }
    }

    stream_type operator ^ (__root)
    {
        if (good()) {
            return stream_type{tree_root};
        } else {
            throw std::runtime_error{"cannot start - no valid state"};
        }
    }

    bool good() const
    {
        return state;
    }

    operator boolean_type() const
    {
        return good() ? &basic_istream_root<Ch>::good : (boolean_type)nullptr;
    }

private:
    bool state = false;
    proptree_type tree_root;
};

using istream_root = basic_istream_root<char>;
using wistream_root = basic_istream_root<wchar_t>;

template<typename Ch> inline 
typename basic_istream_root<Ch>::stream_type operator ^ (basic_istream_root<Ch>& r, const std::string& buffer)
{
    if (!r.open(buffer)) {
        throw std::runtime_error{"failed to read from buffer"};
    }
    return r ^ _root;

}

template<typename Ch> inline 
typename basic_istream_root<Ch>::stream_type operator ^ (basic_istream_root<Ch>& r, std::istream& input)
{
    if (!r.open(input)) {
        throw std::runtime_error{"failed to read from input"};
    }
    return r ^ _root;

}

template<typename Ch> inline 
typename basic_istream_root<Ch>::stream_type operator ^ (basic_istream_root<Ch>& r, const boost::filesystem::path& file_path)
{
    if (!r.open(file_path)) {
        throw std::runtime_error{"failed to read from " + file_path.string()};
    }
    return r ^ _root;

}

template<typename T>
inline basic_istream<T> __child<T>::get()// const 
{
    try {
        return stream.get_child(n);
    } catch (const std::exception& e) {
        if (stream.is_op()) {
            return basic_istream<T>{this->stream.entries(), false};
        } else {
            throw e;
        }
    }

}

typedef basic_istream<char>  istream;
typedef basic_istream<wchar_t> wistream;
typedef __child<wchar_t> _wchild;
typedef __child<char> _child;

namespace detail
{

template<typename T, typename Ch>
struct collection_extractor
{
    typedef typename basic_istream<Ch>::proptree_type   ptree_type;
    typedef typename ptree_type::iterator               iterator;

    static basic_istream<Ch>& process(basic_istream<Ch>& jis, T& container)
    {
        typedef typename T::value_type value_type;
        try {
            for (iterator i = jis.entries().begin(); i != jis.entries().end(); i++) {
                basic_istream<Ch> tmp(i->second);
                value_type new_value = value_type();
                tmp ^ _name("") ^ new_value;
                if (tmp) {
                    container.insert(container.end(),new_value);
                } else {
                    return jis; // failed
                }
            }
        } catch (const std::exception& ) {
            return jis;
        }
        return jis;
    }
};

}   // end of namespace detail

// helper functions that would allow to extract data from collection
template<typename T, typename A, typename Ch>
inline basic_istream<Ch>& operator ^ (basic_istream<Ch>& jis, std::vector<T, A>& container)
{
    return  detail::collection_extractor<std::vector<T, A>, Ch>::process(jis, container);
}

template<typename T, typename A, typename Ch>
inline basic_istream<Ch>& operator ^ (basic_istream<Ch>& jis, std::list<T, A>& container)
{
    return  detail::collection_extractor<std::list<T, A>, Ch>::process(jis, container);
}

template<typename T, typename A, typename Ch>
inline basic_istream<Ch>& operator ^ (basic_istream<Ch>& jis, std::set<T, A>& container)
{
    return  detail::collection_extractor<std::set<T, A>, Ch>::process(jis, container);
}


}   // end of namespace json

