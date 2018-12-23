#include "command_action_reader.hpp"
#include "command_fix_actions.h"
#include "norm_command_action.h"
//#include "mapval_command_action.h"
#include "libs/cleanup_operatiosn/command_processing.h"
#include "libs/cleanup_operatiosn/dataclean_conf.h"
#include "libs/cleanup_operatiosn/missingfix_task.h"
#include "libs/cleanup_operatiosn/fillinvalid_conf.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include "libs/rows_drops/filterout_conf.h"
#include "libs/json_parser/json_istream.h"
#include "libs/system/exceptions.h"
#include "logging.h"
#include <limits>
#include <boost/assign.hpp>
#include <algorithm>
#include <map>

namespace
{

using processing_function = bool (*)(json::istream&, action_reader&);
dataclean_conf_preliminery process_input(json::istream ji);
json::istream& operator ^ (json::istream& js, dataclean_conf_preliminery& dc);
json::istream& operator ^ (json::istream& ji, action_reader& conf);
json::istream& operator ^ (json::istream& ji, filterout_conf& conf);
void read_actions(json::istream& is, dataclean_conf_preliminery& args);
processing_function process_entry(const std::string action_name);
bool drop_action(json::istream& is, action_reader& ac);
//bool mapvals_action(json::istream& is, action_reader& ac);
bool invalid_entry(json::istream&, action_reader&);
void do_insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const action_reader& new_action);

processing_function process_entry(const std::string action_name)
{
    using name2actions = std::map<std::string, processing_function>;
    static const name2actions action_proc = 
        boost::assign::map_list_of("filter.rows", drop_action)
                                  ("fix.missing", fix_action)
           //                       ("map.values", mapvals_action)
                                  ("normalized", normalized_action);

    auto i = action_proc.find(action_name);
    return i == std::end(action_proc) ? invalid_entry : i->second;
}

dataclean_conf_preliminery process_input(json::istream ji)
{
    dataclean_conf_preliminery conf;
    ji ^ conf;
    return conf;
}

json::istream& operator ^ (json::istream& js, dataclean_conf_preliminery& dc)
{
    using namespace json::literals;

    auto p = js ^ json::_child(js, "datasource"_n);
    if (p) {
        p ^ "path"_n ^ dc.source_location;
        if (dc.source_location.empty()) {
            LOG_ERROR<<"missing source location path";
            throw std::runtime_error{"missing source location path"};
        }

        auto ex = js ^ json::_child(js, "export"_n);
        if (ex) {
            ex ^ "path"_n ^ dc.export_file;
            if (dc.export_file.empty()) {
                LOG_ERROR<<"missing export path";
                throw std::runtime_error{"missing export path"};
            }
        } else {
            LOG_ERROR<<"missing export entry in configuration";
            throw std::runtime_error{"missing export entry"};
        }

        auto actions = js ^ json::_child(js, "actions"_n);
        read_actions(actions, dc);
        if (dc.actions.empty()) {
            LOG_WARNING<<"no actions found in configuration";
            //throw std::runtime_error{"no actions were found in configuration"};
        }
        LOG_INFO<<"finish reading actions:\n"<<dc;
    } else {
        LOG_ERROR<<"failed to read datasource entry from JSON";
        throw std::runtime_error{"missing data source entry"};
    }
    return js;
}

json::istream& operator ^ (json::istream& ji, filterout_conf& conf)
{
    using namespace json::literals;

    auto att = ji ^ json::_child(ji, "attr"_n);
    att ^ "colid"_n ^ conf.column;
    auto r = att ^ json::_child(att, "rows"_n);
    r  ^ conf.to_remove;
    std::sort(std::begin(conf.to_remove), std::end(conf.to_remove));
    return ji;
}

json::istream& operator ^ (json::istream& is, action_reader& ac)
{
    using namespace json::literals;

    std::string aname;

    is ^ "action"_n ^ aname;
    auto cf = process_entry(aname);
    LOG_DEBUG<<"processing action "<<aname;
    if (cf(is, ac)) {
        LOG_INFO<<"successfully created action "<<aname;
    } else {
        throw json_error{"invalid action name " + aname};
    }
    return is;
}

void read_actions(json::istream& is, dataclean_conf_preliminery& args)
{
    using namespace json::literals;

    std::for_each(is.entries().begin(), is.entries().end(),
            [&args] (auto& leaf) {
                json::istream tmp(leaf.second);
                action_reader new_action;
                tmp ^ ""_n ^ new_action;
                if (tmp) {
                    do_insert(args, column_id(new_action), new_action);
                }
            }
    );
}
#if 0
bool mapvals_action(json::istream& is, action_reader& ac)
{
    return process_mapval_action(is, ac);
}
#endif

bool drop_action(json::istream& is, action_reader& ac)
{
    filterout_conf input;
    is ^ input;
    if (is) {
        ac.action = action_reader::DROP;
        ac.data.id() = input.column;
        ac.data.drop() = preprocessed_action::drop_action(input);
        return true;
    } else {
        return false;
    }
}
bool invalid_entry(json::istream&, action_reader&)
{
    return false;
}
void do_insert(dataclean_conf_preliminery& to, 
        const dataclean_conf_preliminery::id_type& id,
        const action_reader& new_action)
{
    if (new_action.action == action_reader::NOT_SET) {
        LOG_ERROR<<"error trying to save unknown type of action ("<<new_action.data<<")";
    } else {
        switch (new_action.action) {
            case action_reader::NA_FIX:
                insert(to, id, new_action.data.fixes());
                break;
            case action_reader::NORM_FIX:
                insert(to, id, new_action.data.normalization());
                break;
            case action_reader::DROP:
                insert(to, id, new_action.data.drop());
                break;
            default:
                LOG_ERROR<<"uknown action type "<<(int)new_action.action;
                break;
        }
    }
}

}   // end of local namespace


dataclean_conf_preliminery read_configuration(const std::string& input)
{
    static const std::size_t FILE_PATH_SIZE = 256;

    json::istream_root root;
    try {
        if (input.size() < FILE_PATH_SIZE) {
            boost::filesystem::path pi{input};
            if (boost::filesystem::exists(pi)) {
                LOG_INFO<<"processing from file "<<input;
                return process_input(root ^ pi);
            } else { 
                LOG_INFO<<"processing from command line";
                return process_input(root ^ input);
            }
        } else {
            LOG_INFO<<"processing from command line";
            return process_input(root ^ input);
        }
    } catch (const std::exception& e) {
        LOG_ERROR<<"failed to parser json input "<<e.what();
        throw json_error(e.what());
    }
}

dataclean_conf finilized_configuration(dataclean_conf_preliminery&& prem_conf,
        const schema_type& schema)
{
    return dataclean_conf{std::move(prem_conf), schema};
}

dataclean_conf process_configuration(const std::string& arg)
{
    LOG_INFO<<"command input is\n"<<arg<<"\n";
    auto conf = read_configuration(arg);
    const std::string schema_path = conf.source_location +  "/numeric_table/numeric_table";
    std::ifstream sh_input{schema_path};
    return finilized_configuration(std::move(conf), get_schema(sh_input)); 
}

