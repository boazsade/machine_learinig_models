#include "apps/detect_duplicates/duplication_task.h"
#include "apps/detect_duplicates/duplication_action.h"
#include "apps/detect_duplicates/command_processing.h"
#include "libs/sources_handling/input_data.h"
#include "apps/detect_duplicates/exceptions.h"
#include "logging.h"
#include <boost/filesystem.hpp>
#include <future>
#include <thread>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace 
{

struct filter_dup 
{
    policy_action                   action;
    boost::filesystem::path         source_path;
    column_id                       col_id;

    filter_dup(const duplication_conf& action_conf, 
            const boost::filesystem::path& source);

    duplication_action create_action(const duplication_conf& conf); 
};


filter_dup::filter_dup(const duplication_conf& action_conf, const boost::filesystem::path& source) :
        action(create_action(action_conf)),
        source_path(source),
        col_id(action_conf.col_id)
{
        if (boost::filesystem::exists(source_path)) {
            source_path /= col_id;
            if (boost::filesystem::exists(source_path)) {
                source_path /= col_id;
                if (!boost::filesystem::exists(source_path)) {
                    throw no_such_path{source_path.string()};
                }
            } else {
                throw no_such_path{source_path.string()};
            }
        } else {
            throw no_such_path{source_path.string()};
        }
}

duplication_action filter_dup::create_action(const duplication_conf& conf)
{
        if (conf.act == duplication_conf::KEEP_LAST) {
            return duplication_action{duplication_last{conf.res}};
        } else {
            return duplication_action{duplication_first{conf.res}};
        }
}

std::future<dupliations_report> run_task(const boost::filesystem::path& location, const duplication_conf& conf);
dupliations_report find_duplicates(filter_dup policy);

std::future<dupliations_report> run_task(const boost::filesystem::path& location, const duplication_conf& conf)
{
    LOG_INFO<<"running task for column "<<conf.col_id;
    filter_dup ad{conf, location};
    return std::async(std::launch::async, [ad]() { return find_duplicates(ad); } );
}

dupliations_report find_duplicates(filter_dup policy)
{
    std::ifstream input{policy.source_path.string()};
    if (!input) { 
        LOG_ERROR<<"failed to open "<<policy.source_path.string()<<" for reading";
        throw access_denied{policy.source_path.string()};
    }
    LOG_INFO<<"task for "<<policy.col_id<<" started";
    std::size_t total = 0;

    std::for_each(std::istream_iterator<input_data>(input), std::istream_iterator<input_data>(),
            [&total, &policy](auto data) {
                ++total;
                ::apply_action(policy.action, data);
            }
    );

    dupliations_report ret;
    ret.path = policy.source_path.string();
    ret.total = total;
    ret.column = policy.col_id;
    finding(policy.action, ret.dups, ret.missings);
    LOG_INFO<<"finish processing input at "<<policy.source_path<<" for column "
        <<policy.col_id<<" with "<<total<<" rows and "<<duplications_count(ret)
        <<" duplications and we have  "<<ret.missings.size()<<" missing values";
    return ret;
}

}   // end of local namespace 

all_duplications_report find_duplications(const app_args& args)
{
    // for each of the of the entries we would create an action and then 
    // run it in each own thread
    LOG_INFO<<"starting to process actions ("<<args.duplications.size()<<")";
    using result_type = std::future<dupliations_report>;
    using tasks_results = std::vector<result_type>;

    all_duplications_report report{args.duplications.size()};
    tasks_results results;
    for (auto i = 0u; i < report.size(); ++i) {
        LOG_INFO<<"running async task for column "<<args.duplications[i].col_id;
        results.push_back(run_task(args.input_path, args.duplications[i]));
    }
    LOG_INFO<<"all tasks were started, waiting for them to finish";
    for (auto i = 0u; i < report.size(); ++i) {
        report[i] = results[i].get();
    }
    LOG_INFO<<"finish running all processing";
    return report;
}

std::ostream& operator << (std::ostream& os, const dupliations_report& report)
{
    os<<static_cast<const column_stats&>(report)<<"\nduplications:\n";
    std::copy(report.dups_begin(), report.dups_end(), std::ostream_iterator<row_numbers::value_type>(os, ", "));
    return os;
}

dupliations_report::iterator dupliations_report::dups_begin()
{
    return dups.begin();
}
dupliations_report::iterator dupliations_report::dups_end()
{
    return dups.end();
}

dupliations_report::const_iterator dupliations_report::dups_begin() const
{
    return dups.begin();
}

dupliations_report::const_iterator dupliations_report::dups_end() const
{
    return dups.end();
}

bool no_duplications(const dupliations_report& dr)
{
    return dr.dups.empty();
}

std::size_t duplications_count(const dupliations_report& dr)
{
    return dr.dups.size();
}

void duplicated_row(dupliations_report& to, row_numbers::value_type row_numer)
{
    to.dups.push_back(row_numer);
}

