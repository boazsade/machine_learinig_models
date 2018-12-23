#include "libs/cleanup_operatiosn/cleanup_tasks.h"
#include "libs/cleanup_operatiosn/column_task.h"
#include "clean_output_prep.hpp"
#include "logging.h"
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <thread>
#include <future>
#include <iterator>

namespace
{
    using result_type = std::future<column_task::result_type>;
    using export_data_t = std::vector<source_data>;

    bool validate_output(const export_data_t& output);

    void export_headers(const export_data_t& data, 
            std::ostream& output);

    void export_entries(const export_data_t& data, 
            std::ostream& output);

    void print_row(unsigned int row_num, const export_data_t& columns,
            std::ostream& out);

    cleanup_results process(const cleanup_task::arg_type& args);
    bool validate_results(const details::tasks_results&);

    result_type 
    run_task(const boost::filesystem::path& location, 
            const column_task::arg_type& args);

    bool export_data(const boost::filesystem::path& to, 
            const details::tasks_results& columns);

    cleanup_results process(const cleanup_task::arg_type& args)
    {
        using tasks_list = std::vector<result_type>;
    
        tasks_list tasks;
        LOG_INFO<<"start running processing tasks";
        boost::filesystem::path source_path{args.source_location};
        std::transform(begin(args), end(args), std::back_inserter(tasks), 
                        [&source_path](auto& task_conf) {
                                return run_task(source_path, task_conf);
                            }
                    ); 
        LOG_INFO<<"waiting for all "<<tasks.size()<<" tasks to complete";
        details::tasks_results clean_columns;
        std::transform(std::begin(tasks), std::end(tasks), 
                std::back_inserter(clean_columns),
                [](auto& task) {
                    return task.get();
                }
            );
        if (clean_columns.empty() || !validate_results(clean_columns)) {
            if (clean_columns.empty()) {
                LOG_ERROR<<"no data from the clean operations!!";
            } else {
                LOG_ERROR<<"one of the column is invalid state - failed internally - cannot export!!";
            }
            return cleanup_results{};
        }
        
        if (export_data(args.export_file, clean_columns)) {
            return details::make_results(std::move(clean_columns));
        } else {
            LOG_ERROR<<"fail to export results to "<<args.export_file<<" reporting error";
            return details::make_error_results();
        }
        
    }

    bool validate_results(const details::tasks_results& tr)
    {
        return std::none_of(std::begin(tr), std::end(tr), [](auto& entry) {
                        LOG_DEBUG<<"check column "<<data(entry).id()<<", number of entries "<<size(data(entry));
                        if (data(entry).id().empty()) {
                            LOG_INFO<<"found an empty entry in the results column!!";
                            return true;
                        }
                        if (empty(data(entry))) {
                            LOG_ERROR<<"found empty column '"<<data(entry).id()<<"'";
                            return true;
                        }
                        return false;
                        //return empty(data(entry));
                    }
        );
    }

    result_type 
    run_task(const boost::filesystem::path& location, 
            const column_task::arg_type& args)
    {
        LOG_INFO<<"starting task with actions\n"<<args<<"\nat '"<<location<<"'"; 
        column_task task;

        return std::async(std::launch::async, [location, args, task]() { 
                return task(args, location); 
            }
        );
    }

    bool export_data(const boost::filesystem::path& to, 
            const details::tasks_results& columns)
    {
        export_data_t out_data;
        std::transform(std::begin(columns), std::end(columns), std::back_inserter(out_data), 
                [](auto& col) {
                    return data(col);
                }
        );
        if (!validate_output(out_data)) {
            return false;
        }

        std::ofstream output{to.string()};
        if (!output) {
            LOG_ERROR<<"failed to open '"<<to<<"' for writing, cannot export data";
            return false;
        } else {
            export_headers(out_data, output);
            // now the actual data
            export_entries(out_data, output);
            return true;
        }
    }

    void export_headers(const export_data_t& columns, 
            std::ostream& output)
    {
        output<<"\""<<columns[0].id()<<"\"";
        auto i = std::begin(columns);
        ++i; 
        // headers
        std::for_each(i, std::end(columns), [&output](auto& col) {
                output<<",\""<<col.id()<<"\"";   
            }
        );
        output<<",\"index\"\n";
    }

    void export_entries(const export_data_t& data, 
            std::ostream& output)
    {
        const auto totalr = data[0].row_data().size();

        for (auto i = 0u; i < totalr; ++i) {
            print_row(i, data, output);
        }
        output<<"\n";
    }

    void print_row(unsigned int row_num, const export_data_t& columns,
            std::ostream& out)
    {
        auto beg = std::begin(columns);
        out<<columns[0].row_data()[row_num].value;
        auto row = columns[0].row_data()[row_num].row;
        auto e = std::end(columns); 
        if (std::next(beg) != e) {
            ++beg;
            std::for_each(beg, e, [&out, row_num, row] (const auto& col) {
                    if (col.row_data()[row_num].row == row) {
                        out<<","<<col.row_data()[row_num].value;
                    } else {
                        LOG_ERROR<<"column "<<col.id()<<" has row number "
                            <<col.row_data()[row_num].row
                            <<" that do not match expected row number "<<row;
                        throw std::runtime_error{"internal error - columns with unmatch row numbers"};
                    }
                }
            );
        }
        out<<","<<beg->row_data()[row_num].row<<"\n";

    }

    bool validate_output(const export_data_t& output)
    {
        auto es = output[0].row_data().size();
        auto i = std::find_if(std::begin(output), std::end(output), 
                              [es](auto& entry) {
                                LOG_INFO<<"id: "<<entry.id()<<" number of rows "<<entry.row_data().size()<<" expected "<<es;
                                return entry.row_data().size() != es;
                        }
            );
        if (i != std::end(output)) {
            LOG_ERROR<<"not all output colunns have the same enties ("<<i->id()<<")";
            throw std::runtime_error{"cannot export - one of the column is invalid (fail to appliy operations on it"};
        } else {
            return true;
        }
    }
}   // end of local namespace

cleanup_task::result_type cleanup_task::operator()(const arg_type& args) const
{
    if (boost::filesystem::exists(args.source_location)) {
        boost::filesystem::path exp(args.export_file);
        if (!boost::filesystem::exists(exp.parent_path())) {
            LOG_ERROR<<"cannot export to "<<exp;
            throw std::runtime_error{"no such dirctory " + exp.parent_path().string()};
        }
        return process(args);
    } else {
        LOG_ERROR<<"invalid path given for data processing '"
            <<args.source_location<<"', would not process";
        throw std::runtime_error{"not such path " + args.source_location};
    }
}

