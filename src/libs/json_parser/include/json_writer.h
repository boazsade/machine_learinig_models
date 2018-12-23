#pragma once
#include "json_base.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>
#include <utility>      // std::pair
#include <boost/mpl/if.hpp> // boost::mpl::if_c
#include <boost/type_traits/is_pointer.hpp> // boost::is_pointer
#include <boost/type_traits/remove_pointer.hpp> // boost::remove_pointer


namespace json
{

struct array_writer;
struct warray_writer;

struct entry_writer
{

    entry_writer();

    void add_subnode(const array_writer& writer);

    void add_subnode(const array_writer& writer, const char* name);

    void add_subnode(const entry_writer& writer);

    void add_subnode(const entry_writer& writer, const char* name);

    template<typename T>
    explicit entry_writer(const array_data<T>& entry) 
    {
        this->add(entry);
    }

    template<typename T>
    explicit entry_writer(const entry<T>& entry) 
    {
        this->add<T>(entry);
    }

    template<typename T>
    entry_writer(const char* n, const T& val)
    {
        this->add<T>(n, val);
    }

    template<typename T>
    void add(const char* n, const T& val)
    {
        entry<T> e(n, val);
        e.write(child);
    }

    template<typename T>
    void add(const array_data<T>& entry)
    {
        entry.write(child);
    }

    template<typename T>
    void add(const entry<T>& ent)
    {
        ent.write(child);
    }    

    boost::property_tree::ptree& node();    

    const boost::property_tree::ptree& node() const;    

private:
    boost::property_tree::ptree child;
};

///////////////////////////////////////////////////////////////////////////////
// wide version
struct wentry_writer
{
    wentry_writer();

    void add_subnode(const warray_writer& writer);

    void add_subnode(const warray_writer& writer, const char* name);

    void add_subnode(const wentry_writer& writer);

    void add_subnode(const wentry_writer& writer, const char* name);

    template<typename T>
    explicit wentry_writer(const array_data<T>& entry) 
    {
        this->add(entry);
    }

    template<typename T>
    explicit wentry_writer(const entry<T>& entry) 
    {
        this->add<T>(entry);
    }

    template<typename T>
    wentry_writer(const char* n, const T& val)
    {
        this->add<T>(n, val);
    }

    template<typename T>
    void add(const char* n, const T& val)
    {
        entry<T> e(n, val);
        e.write(child);
    }

    template<typename T>
    void add(const array_data<T>& entry)
    {
        entry.write(child);
    }

    template<typename T>
    void add(const entry<T>& ent)
    {
        ent.write(child);
    }    

    boost::property_tree::wptree& node();    

    const boost::property_tree::wptree& node() const;    

private:
    boost::property_tree::wptree child;
};

///////////////////////////////////////////////////////////////////////////////
struct array_writer
{
    array_writer();
    

    array_writer(const entry_writer& writer, const char* name = "");

    template<typename T>
    array_writer(const char* name, const T& val)
    {
        add(name, val);
    }


    void add(const entry_writer& writer, const char* name = "");    

    void add(const array_writer& arr, const char* name = "");    

    template<typename T>
    void add(const char* name, const T& val)
    {
        entry_writer writer(name, val);
        nodes.push_back(std::make_pair(name, writer.node()));
    }

    boost::property_tree::ptree& get_nodes();
   

    const boost::property_tree::ptree& get_nodes() const;    

    template<typename Iter>
    void save(Iter from, Iter to)
    {
        while (from != to) {
            add(*from);
            ++from;
        }
    }

private:
    boost::property_tree::ptree nodes;
};

/////////////////////////
// wide version
struct warray_writer
{
    warray_writer();
    

    warray_writer(const wentry_writer& writer, const wchar_t* name = 0);

    template<typename T>
    warray_writer(const char* name, const T& val)
    {
        add(name, val);
    }


    void add(const wentry_writer& writer, const wchar_t* name = 0);    

    void add(const warray_writer& arr, const wchar_t* name = 0);    

    template<typename T>
    void add(const wchar_t* name, const T& val)
    {
        wentry_writer writer(name, val);
        nodes.push_back(std::make_pair(name, writer.node()));
    }

    boost::property_tree::wptree& get_nodes();
   

    const boost::property_tree::wptree& get_nodes() const;    

    template<typename Iter>
    void save(Iter from, Iter to)
    {
        while (from != to) {
            add(*from);
            ++from;
        }
    }

private:
    boost::property_tree::wptree nodes;
};

///////////////////////////////////////////////////////////////////////////////
struct generate_array
{
    generate_array();
    
    generate_array(const char* name, array_writer& arr);
    
    template<typename T>
    generate_array(const entry<T>& node, const char* name = "")
    {
        write<T>(name, node);
    }

    void write(const char* name, array_writer& arr);    

    template<typename T>
    void write(const entry<T>& node)
    {
        root.put(node.name, node.value);
    }

   
    boost::property_tree::ptree& get_root();
    
    const boost::property_tree::ptree& get_root() const;

private:
    boost::property_tree::ptree root;
};

/////////////////////////
// wide version
struct wgenerate_array
{
    wgenerate_array();
    
    wgenerate_array(const char* name, warray_writer& arr);
    
    template<typename T>
    wgenerate_array(const entry<T>& node, const char* name = "")
    {
        write<T>(name, node);
    }

    void write(const char* name, warray_writer& arr);    

    template<typename T>
    void write(const entry<T>& node)
    {
        root.put(widen_str(node.name), node.value);
    }

   
    boost::property_tree::wptree& get_root();
    
    const boost::property_tree::wptree& get_root() const;

private:
    boost::property_tree::wptree root;
};

///////////////////////////////////////////////////////////////////////////////

namespace details
{
// this would be used for write a list of items from the same type
// with range being controled by pointers

template <typename T>
struct pointer_setup
{
    typedef typename boost::remove_pointer<T>::type  value_type;
    typedef T                                        iterator_type; 
};

template <typename T>
struct interator_setup
{
    typedef T                       iterator_type;
    typedef typename T::value_type  value_type;
};

struct _wide_char_setup
{
    typedef json::wentry_writer      entry_type;
    typedef json::wgenerate_array    generate_type;
    typedef std::wstring             result_type;
};

struct _char_setup
{
    typedef json::entry_writer      entry_type;
    typedef json::generate_array    generate_type;
    typedef std::string             result_type;
};

template<bool WideChar>
struct writer_setup
{
    typedef typename boost::mpl::if_c<WideChar,
                                      _wide_char_setup,
                                      _char_setup
                            >::type   type;
};

template<typename IterSetup, bool WC>
struct iteration_writing
{
    typedef typename writer_setup<WC>::type charset_setup;
    typedef IterSetup                       iter_setup;
    typedef typename charset_setup::result_type result_type;

    typedef typename iter_setup::iterator_type  iterator_type;
    typedef typename iter_setup::value_type     value_type;

    static result_type run(const char* base, iterator_type from, iterator_type to)
    {
        typedef typename charset_setup::entry_type entry_type;
        typedef typename charset_setup::generate_type generate_type;
        json::array_writer aw;
        while (from != to) {
            array_data<value_type>  entry(*from);
            entry_type node(entry);
            aw.add(node);
            ++from;
        }

        generate_type root(base, aw);
        return write(root);

    }
};

template<typename T, bool WC>
struct pointer_writer
{
    typedef pointer_setup<T>                                    setup;
    typedef iteration_writing<setup, WC>                        writer_type;
    typedef typename iteration_writing<setup, WC>::result_type  result_type;
    typedef typename iteration_writing<setup, WC>::iterator_type iterator_type;
    static result_type run(const char* base, iterator_type from, iterator_type to)
    {
        return writer_type::run(base, from, to);
    }

};

template<typename T, bool WC>
struct iterator_writer
{
    typedef interator_setup<T>                                  setup;
    typedef iteration_writing<setup, WC>                        writer_type;
    typedef typename iteration_writing<setup, WC>::result_type  result_type;
    typedef typename iteration_writing<setup, WC>::iterator_type iterator_type;
    static result_type run(const char* base, iterator_type from, iterator_type to)
    {
        return writer_type::run(base, from, to);
    }
};

}   // end of namespace details

template<typename Iter>
std::string write_array(const char* base, Iter from, Iter to)
{
    typedef typename boost::mpl::if_c<boost::is_pointer<Iter>::value,
                                      details::pointer_writer<Iter, false>,
                                      details::iterator_writer<Iter, false>
                            >::type     writer_type;

    return writer_type::run(base, from, to);
}

template<typename Iter>
std::string wwrite_array(const char* base, Iter from, Iter to)
{
    typedef typename boost::mpl::if_c<boost::is_pointer<Iter>::value,
                                      details::pointer_writer<Iter, true>,
                                      details::iterator_writer<Iter, true>
                            >::type     writer_type;

    return writer_type::run(base, from, to);
}

}   // end of namespace json

