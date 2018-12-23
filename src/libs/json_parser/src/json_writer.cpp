#include "libs/json_parser/json_writer.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"

namespace json
{

namespace
{

bool write_to(std::ostream& to, const boost::property_tree::ptree& pt, bool indent)
{
    try {
        write_json(to, pt, indent);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool write_to(std::wostream& to, const boost::property_tree::wptree& pt, bool indent)
{
    try {
        write_json(to, pt, indent);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
    
}   // end of local namespace

sub_tree::sub_tree(const std::string& st) : entry(st)
{

}

entry_writer::entry_writer()
{
}

boost::property_tree::ptree& entry_writer::node()
{
    return child;
}

const boost::property_tree::ptree& entry_writer::node() const
{
    return child;
}

void entry_writer::add_subnode(const array_writer& writer)
{
    child.push_back(std::make_pair("", writer.get_nodes()));
}

void entry_writer::add_subnode(const array_writer& writer, const char* name)
{
    child.push_back(std::make_pair(name, writer.get_nodes()));
}

void entry_writer::add_subnode(const entry_writer& writer)
{
    child.push_back(std::make_pair("", writer.node()));
}

void entry_writer::add_subnode(const entry_writer& writer, const char* name)
{
    child.push_back(std::make_pair(name, writer.node()));
}

///////////////////////////////////////////////////////////////////////////////

wentry_writer::wentry_writer()
{
}

boost::property_tree::wptree& wentry_writer::node()
{
    return child;
}

const boost::property_tree::wptree& wentry_writer::node() const
{
    return child;
}

void wentry_writer::add_subnode(const warray_writer& writer)
{
    child.push_back(std::make_pair(widen_str(""), writer.get_nodes()));
}

void wentry_writer::add_subnode(const warray_writer& writer, const char* name)
{
    child.push_back(std::make_pair(widen_str(name), writer.get_nodes()));
}

void wentry_writer::add_subnode(const wentry_writer& writer)
{
    child.push_back(std::make_pair(widen_str(""), writer.node()));
}

void wentry_writer::add_subnode(const wentry_writer& writer, const char* name)
{
    child.push_back(std::make_pair(widen_str(name), writer.node()));
}

///////////////////////////////////////////////////////////////////////////////

array_writer::array_writer()
{
}

array_writer::array_writer(const entry_writer& writer, const char* name)
{
    add(writer, name);   
}

void array_writer::add(const entry_writer& writer, const char* name)
{
    nodes.push_back(std::make_pair(name, writer.node()));
}

void array_writer::add(const array_writer& arr, const char* name)
{
    nodes.push_back(std::make_pair(name, arr.get_nodes()));
}

boost::property_tree::ptree& array_writer::get_nodes()
{
    return nodes;
}

const boost::property_tree::ptree& array_writer::get_nodes() const
{
    return nodes;
}

///////////////////////////////////////////////////////////////////////////////

warray_writer::warray_writer()
{
}

warray_writer::warray_writer(const wentry_writer& writer, const wchar_t* name)
{
    add(writer, name);   
}

void warray_writer::add(const wentry_writer& writer, const wchar_t* name)
{
    nodes.push_back(std::make_pair(name, writer.node()));
}

void warray_writer::add(const warray_writer& arr, const wchar_t* name)
{
    nodes.push_back(std::make_pair(name, arr.get_nodes()));
}

boost::property_tree::wptree& warray_writer::get_nodes()
{
    return nodes;
}

const boost::property_tree::wptree& warray_writer::get_nodes() const
{
    return nodes;
}

///////////////////////////////////////////////////////////////////////////////

generate_array::generate_array()
{
}

generate_array::generate_array(const char* name, array_writer& arr)
{
    write(name, arr);
}

void generate_array::write(const char* name, array_writer& arr)
{
    root.add_child(name, arr.get_nodes());
}

boost::property_tree::ptree& generate_array::get_root()
{
    return root;
}

const boost::property_tree::ptree& generate_array::get_root() const
{
    return root;
}

///////////////////////////////////////////////////////////////////////////////

wgenerate_array::wgenerate_array()
{
}

wgenerate_array::wgenerate_array(const char* name, warray_writer& arr)
{
    write(name, arr);
}

void wgenerate_array::write(const char* name, warray_writer& arr)
{
    root.add_child(widen_str(name), arr.get_nodes());
}

boost::property_tree::wptree& wgenerate_array::get_root()
{
    return root;
}

const boost::property_tree::wptree& wgenerate_array::get_root() const
{
    return root;
}

///////////////////////////////////////////////////////////////////////////////

bool write(std::ostream& to, generate_array& ga, bool indent)
{
    try {
        write_json(to, ga.get_root(), indent);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}


bool write(std::ostream& to, array_writer& ga, bool indent)
{
    return write_to(to, ga.get_nodes(), indent);
}

std::string write(ostream& os, bool indent)
{
    std::ostringstream s;
    if (write_to(s, os.entries(), indent)) {
        return s.str();
    } else {
        const std::string error = std::string();
        return error;
    }
}

bool write(std::ostream& to, entry_writer& ga, bool indent)
{
    try {
        write_json(to, ga.node(), indent);
        return true;
    } catch (const std::exception&) {        
        return false;
    }
}


std::string write(generate_array& ga, bool indent)
{
    std::ostringstream target;
    if (json::write(target, ga, indent)) {
        return target.str();
    } else {
        static const std::string err = std::string();
        return err;
    }
}

void write(generate_array& ga, std::string& arg, bool indent)
{
	std::ostringstream target;
    if (json::write(target, ga, indent))
        arg.assign(target.str());
}


std::string write(array_writer& ga, bool indent)
{
    std::ostringstream target;
    if (json::write(target, ga, indent)) {
        return target.str();
    } else {
        static const std::string err = std::string();
        return err;
    }
}


std::string write(entry_writer& ga, bool indent)
{
    std::ostringstream target;
    if (json::write(target, ga, indent)) {
        return target.str();
    } else {
        static const std::string err = std::string();
        return err;
    }
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

std::wstring wwrite(wostream& os, bool indent)
{
    std::wostringstream s;
    if (write_to(s, os.entries(), indent)) {
        return s.str();
    } else {
        static const std::wstring error = std::wstring();
        return error;
    }
}

bool wwrite(std::wostream& to, wgenerate_array& ga, bool indent)
{
    return write_to(to, ga.get_root(), indent);
}


bool wwrite(std::wostream& to, warray_writer& ga, bool indent)
{
    try {
        write_json(to, ga.get_nodes(), indent);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}


bool wwrite(std::wostream& to, wentry_writer& ga, bool indent)
{
    try {
        write_json(to, ga.node(), indent);
        return true;
    } catch (const std::exception&) {        
        return false;
    }
}


std::wstring wwrite(wgenerate_array& ga, bool indent)
{
    std::wostringstream target;
    if (json::wwrite(target, ga, indent)) {
        return target.str();
    } else {
        static const std::wstring err = std::wstring();
        return err;
    }
}


std::wstring wwrite(warray_writer& ga, bool indent)
{
    std::wostringstream target;
    if (json::wwrite(target, ga, indent)) {
        return target.str();
    } else {
        static const std::wstring err = std::wstring();
        return err;
    }
}


std::wstring wwrite(wentry_writer& ga, bool indent)
{ 
    std::wostringstream target;
    if (json::wwrite(target, ga, indent)) {
        return target.str();
    } else {
        static const std::wstring err = std::wstring();
        return err;
    }
}


std::string as_string(const istream& input)
{
    std::ostringstream buffer;
    if (json::write_to(buffer, input.entries(), false)) {
        return buffer.str();
    } else {
        static const std::string error = std::string();
        return error;
    }
}

namespace detail
{

std::string impl2string<char>::write(basic_output_stream<char>& input) {
    std::ostringstream s;
    write_to(s, input.tree_root, false);
    return s.str();
}

std::wstring impl2string<wchar_t>::write(basic_output_stream<wchar_t>& input) {
    std::wostringstream s;
    write_to(s, input.tree_root, false);
    return s.str();
}

}   // end of namespace detail

}   // end of namespace json

