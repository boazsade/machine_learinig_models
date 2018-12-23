#include "apps/detect_duplicates/command_processing.h"
#include "apps/detect_duplicates/duplication_task.h"
#include "apps/detect_duplicates/exceptions.h"
#include "logging.h"
#include "libs/json_parser/jsonfwrd.h"
#include "libs/json_parser/json_istream.h"

struct action_entry {
    explicit action_entry(app_args::duplication_actions& da,
            app_args::outliers_actions& oa) :
        daction(da), oaction(oa)
    {
    }

    app_args::duplication_actions& daction;
    app_args::outliers_actions& oaction;
};

void parse(const std::string& input, app_args& args);
void parse(const boost::filesystem::path& input, app_args& args);
void read_conf(json::istream& is, app_args& args);
json::istream& duplication_action(json::istream& is, app_args::duplication_actions& da);
json::istream& outliers_action(json::istream& is, app_args::outliers_actions& oa);
json::istream& operator ^ (json::istream& is, app_args& act);
json::istream& operator ^ (json::istream& is, duplication_conf& act);
json::istream& operator ^ (json::istream& is, outliers_conf& olc);
json::istream& operator ^ (json::istream& is, action_entry& olc);

bool parse_command(const std::string& input, app_args& args)
{
    LOG_INFO<<"processing input ' "<<input<<" '";
    static const std::size_t path_len = 256;
    if (input.size() >  path_len) { // assume this is an input
        LOG_INFO<<"reading input as command line input";
        parse(input, args);
    } else {
        LOG_INFO<<"reading input from a file: "<< input;
        const auto p = boost::filesystem::path(input); 
        if (boost::filesystem::exists(p)) {
            LOG_INFO<<"processing input from "<<input;
            parse(p, args);
        } else {
            LOG_INFO<<"processing from command line";
            parse(input, args);
        }
    }
    LOG_INFO<<"configuration:\n"<<args;
    // make sure that we have only valid entries
    auto i = std::remove_if(std::begin(args.duplications), std::end(args.duplications),
            [](auto e) { return e.col_id.empty() || e.act == duplication_conf::NO_DEFINED; }
        );
    args.duplications.erase(i, std::end(args.duplications));
    auto it = std::remove_if(std::begin(args.outliers), std::end(args.outliers),
            [](auto e) { return e.col_id.empty(); } );
    args.outliers.erase(it, std::end(args.outliers));
    // and return back whether we were successfull
    return !(args.duplications.empty() && args.outliers.empty());
}


void parse(const std::string& input, app_args& args)
{
    json::istream_root root;
    root ^ input;
    auto bn = root ^ json::_root;
    bn ^ args;
    
}

void parse(const boost::filesystem::path& input, app_args& args)
{
    json::istream_root root;
    root ^ input;
    auto bn = root ^ json::_root;
    bn ^ args;
}


json::istream& operator ^ (json::istream& is, app_args& act)
{
    using namespace json::literals;
    auto f = is ^ json::_child(is, "datasource"_n);
    if (!f) {
        throw configuration_error{"missing datasource entry in configuration"};
    }
    std::string p;
    f ^ "path"_n ^ p;
    if (p.empty()) {
        throw configuration_error{"missing path for data to read for this program"};
    }
    act.input_path = p;
    auto actions = is ^ json::_child(is, "actions"_n);
    if (!actions) {
        throw configuration_error{"no actions entries in configuration"};
    }
    //actions ^ act.duplications;
    read_conf(actions, act);
    return is;
}

void read_conf(json::istream& is, app_args& args)
{

    using namespace json::literals;

    LOG_INFO<<"reading new action";
    action_entry ae{args.duplications, args.outliers};
    try {
        std::for_each(is.entries().begin(), is.entries().end(), [&ae] (auto& entry) {
                json::istream tmp(entry.second);
                tmp ^ ""_n ^ ae;
            }
        );
    } catch (const std::exception& e) {
        LOG_ERROR<<"error while reading json: "<<e.what();
        throw configuration_error{std::string{"failed to read configureation"} + e.what()};
    }
}

json::istream& operator ^ (json::istream& is, action_entry& olc)
{
    static const std::string DUP_ACTION_NAME =  "filter.dup";
    static const std::string OUTLIER_ACTION_NAME = "outlier.find";
    using namespace json::literals;

    std::string an;
    is ^ "action"_n ^ an;
    LOG_DEBUG<<"processing action "<<an;
    if (an == DUP_ACTION_NAME) {
        duplication_action(is, olc.daction);
    } else if (an == OUTLIER_ACTION_NAME) {
        outliers_action(is, olc.oaction);
    } else {
        LOG_WARNING<<"not a valid action: "<<an;
    }
    return is;
}

json::istream& operator ^ (json::istream& is, duplication_conf& act)
{
    using namespace json::literals;

    auto att = is ^ json::_child(is, "attr"_n);
    std::string cid;
    int r = -1;
    std::string a;
    att ^ "colid"_n ^ cid ^ "resolution"_n ^ r ^ "keep"_n ^ a;
    if (att) {
        act = duplication_conf{cid, r, a};
    }
    return is;
}

json::istream& operator ^ (json::istream& is, outliers_conf& olc)
{
    using namespace json::literals;

    auto att = is ^ json::_child(is, "attr"_n);
    att ^ "colid"_n ^ olc.col_id;
    return is;
}

json::istream& duplication_action(json::istream& is, 
        app_args::duplication_actions& da)
{
    app_args::duplication_actions::value_type act;
    is ^ act;
    if (is) {
        da.push_back(act);
    }
    return is;
}

json::istream& outliers_action(json::istream& is,
        app_args::outliers_actions& oa)
{
    app_args::outliers_actions::value_type act;
    is ^ act;
    if (is) {
        oa.push_back(act);
    }
    return is;
}


std::ostream& operator << (std::ostream& os, const app_args& args)
{
    os<<" data source path: ["<<args.input_path<<"]\nduplication detection actions:\n";
    std::copy(std::begin(args.duplications), std::end(args.duplications), 
            std::ostream_iterator<app_args::duplication_actions::value_type>(os, "\n"));
    os<<"outliers actions:\n";
    std::copy(std::begin(args.outliers), std::end(args.outliers),
            std::ostream_iterator<app_args::outliers_actions::value_type>(os, "\n"));
    return os;
}

