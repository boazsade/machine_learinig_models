#include "apps/detect_duplicates/outliers_task.h"
#include "apps/detect_duplicates/outliers_conf.h"
#include "apps/detect_duplicates/outliers_action.h"
#include "apps/detect_duplicates/exceptions.h"
#include "apps/detect_duplicates/command_processing.h"
#include "logging.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <future>
#include <thread>
#include <iterator>

namespace 
{

void merge(const all_duplications_report&, finiding_summery& current_stats);
finiding_summery::value_type up_cast(const dupliations_report& from);
finiding_summery actions_data(const app_args& args, 
       const all_duplications_report& duplications);

finiding_summery::value_type build_action_data(const app_args::outliers_actions::value_type& ola,
            const app_args::duplication_actions& da, const all_duplications_report& dups,
            const std::string& path);

std::future<outlier_report> run_task(const outlier_report& task_input);

outlier_report detect_outliers(outlier_report report, policy_action action);
std::string make_path(const std::string& bp, const std::string& cn);


finiding_summery actions_data(const app_args& args,
        const all_duplications_report& duplications)
{
    finiding_summery ret;
    const auto& dup_actions = args.duplications;
    const auto& path = args.input_path.string();
    std::for_each(std::begin(args.outliers), std::end(args.outliers), [&duplications, &dup_actions ,&ret, &path] (auto& arg) {
                            ret.push_back(build_action_data(arg, dup_actions, duplications, path));
                        }
    );
    return ret;
}

finiding_summery::value_type build_action_data(const app_args::outliers_actions::value_type& ola,
            const app_args::duplication_actions& da, const all_duplications_report& dups,
            const std::string& path)
{
    auto i = std::find_if(std::begin(da), std::end(da), [&ola](auto& dup_act) {
                return dup_act.col_id == ola.col_id;
            }
        );
    if (i == std::end(da)) {    // this action is unique for outlier
        return outlier_report{ola.col_id, make_path(path, ola.col_id)};
    } else {    // we have 
        auto it = std::find_if(std::begin(dups), std::end(dups), [&ola](auto& d) {
                return d.column == ola.col_id;
            }
        );
        if (it == std::end(dups)) {
            LOG_ERROR<<"running outlier task "<<ola<<" before running duplication detection";
            throw std::runtime_error{std::string{"running outliers before running duplication detection for "} + ola.col_id};
        }
        return up_cast(*it);
    }
}

std::future<outlier_report> run_task(const outlier_report& task_input)
{
    outlier_predicate action{task_input.dups_begin(), task_input.dups_end()};
    return std::async(std::launch::async, [&task_input, action]() {
            return detect_outliers(task_input, action);
        }
    );
}

finiding_summery::value_type up_cast(const dupliations_report& from)
{
    finiding_summery::value_type ret{from.column, from.path};
    ret.total = from.total;
    ret.dups = from.dups;
    ret.missings = from.missings;
    return ret;
}

outlier_report detect_outliers(outlier_report report, policy_action action)
{
    std::ifstream input{report.path};
    if (!input) {
        throw access_denied{report.path};
    }
    outliers_detector detect_action{action};

    auto& total = report.total;
    total = 0;
    std::for_each(std::istream_iterator<input_data>(input), std::istream_iterator<input_data>(),
            [&detect_action, &total](auto& data) {
                ++total;
                ::apply_action(detect_action, data);    
            }
    );
    finding(detect_action, report.outliers, report.missings);
    LOG_INFO<<"found "<<report.outliers.size()<<" outliers along with "<<report.missings.size()<<
        " missing values on column "<<report.column;
    return report;
}

void merge(const all_duplications_report& prev_stats, finiding_summery& current_stats)
{
    std::for_each(std::begin(prev_stats), std::end(prev_stats), [&current_stats] (auto& stat) {
            auto i = std::find_if(std::begin(current_stats), std::end(current_stats), [&stat] (auto& ps) {
                            return ps.column == stat.column;
                        }
            );
            if (i == std::end(current_stats)) {
                current_stats.push_back(up_cast(stat));
            }

        }
    );
         
}

std::string make_path(const std::string& bp, const std::string& cn)
{
    boost::filesystem::path path{bp};
    path /= cn;
    if (!boost::filesystem::exists(path)) {
        throw no_such_path{path.string()};
    }
    path /= cn;
    if (!boost::filesystem::exists(path)) {
        throw no_such_path{path.string()};
    }
    return path.string();
}

}   // end of local namespace

outlier_report::iterator outlier_report::outliers_begin()
{
    return outliers.begin();
}

outlier_report::iterator outlier_report::outliers_end()
{
    return outliers.end();
}

outlier_report::const_iterator outlier_report::outliers_begin() const
{
    return outliers.begin();
}

outlier_report::const_iterator outlier_report::outliers_end() const
{
    return outliers.end();
}

bool no_outliers(const outlier_report& rep)
{
    return rep.outliers.empty();
}

std::size_t outliers_count(const outlier_report& rep)
{
    return rep.outliers.size();
}

void outliered_row(outlier_report& to, row_numbers::value_type row_number)
{
    to.outliers.push_back(row_number);
}

std::ostream& operator << (std::ostream& os, const outlier_report& rep)
{
    os<<static_cast<const dupliations_report&>(rep);
    std::copy(rep.outliers_begin(), rep.outliers_end(), std::ostream_iterator<row_numbers::value_type>(os, ","));
    return os;
}

finiding_summery find_outliers(const app_args& args, 
       const all_duplications_report& duplications)
{
    using result_type = std::future<outlier_report>;
    using tasks_list = std::vector<result_type>;

    finiding_summery actions{actions_data(args, duplications)};    
    tasks_list tasks;
    LOG_INFO<<"starting outliers detection for "<<actions.size()<<" columns";
    std::for_each(std::begin(actions), std::end(actions), [&tasks] (auto& act) {
                tasks.push_back(run_task(act));
            }
    );
    LOG_INFO<<"all outliers detection tasks are running";
    for (auto i = 0u; i < actions.size(); i++) {
        actions[i] = tasks[i].get();
    }
    // once we finish with outliers detection, we must make sure 
    // that we didn't left behind reports that are about duplications
    // so we need to find whether there such as merge them back
    merge(duplications, actions);
    return actions;
}


