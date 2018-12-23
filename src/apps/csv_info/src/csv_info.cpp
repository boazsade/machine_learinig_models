#include "logging.h"
#include "apps/csv_info/csv_info.h"
#include "apps/csv_info/test_date.h"
#include "apps/csv_info/test_number.h"
#include "apps/csv_info/errors_types.h"
#include "libs/csv_parser/csv_parser.h"
#include "libs/csv_parser/csv_iterator.h"
#include <boost/algorithm/string/trim.hpp>
#include <assert.h>

namespace 
{

bool handle_first(csv::cells_iterator<csv::parser> it, csv_info& ci);
bool process_line(csv::cells_iterator<csv::parser> i, csv_info& ci);
bool handle_invalid_count(std::size_t cols, int expected);
void set_guessing(std::string csv_entry, csv_info::entry_type& ci);
const std::string err_msg_begin = "{\"error\": {\"msg\":";
const std::string err_msg_invalid_col_count = "\"invalid_columns_count\",";
const std::string err_msg_invalid_col_payload = "\"payload\": \"in file ";
const std::string err_msg_end = "}}";

bool handle_first(csv::cells_iterator<csv::parser> it, csv_info& ci)
{
    static const std::string err_msg_no_data = "\"no_data_in_file\",";
    static const std::string err_msg_no_data_payload = "\"payload\": \"in file ";

    auto re = csv::cells_iterator<csv::parser>{};
    std::size_t col_count = 0;
    try {
        for (; it != re; ++it) {
            if (col_count == 0) {
                if (!it->empty()) {
                    ++col_count;
                }
            } else {
                ++col_count;
            }
        }
    } catch (...) {
        static const auto err = csv_parsing_error{};
        throw err;
    }
    if (col_count == 0) {
        LOG_WARNING<<"empty file found"<<std::endl;
        static const auto err = empty_input_file{};
        throw err;
    }
    if (ci.expected() < 0) {
        ci.expected((int)col_count);
        return col_count > 0;
    } else {
        if (ci.expected() != (int)col_count) {
            return handle_invalid_count(col_count, ci.expected());
        } else {
            return true;
        }
    }
}

bool handle_invalid_count(std::size_t cols, int expected)
{
    throw invalid_columns_count{(std::size_t)expected, cols};
    return false;
}

bool process_line(csv::cells_iterator<csv::parser> i, csv_info& ci)
{
    auto count = 0u;
    auto re = csv::cells_iterator<csv::parser>{};
   
    for (; i != re; ++i) {
        LOG_DEBUG<<"processing column "<<count<<" value '"<<*i<<"'\n";
        if (count >= ci.columns_count()) {
            return handle_invalid_count(count+1, ci.expected());
        } else {
            set_guessing(*i, ci[count]);
            ++count;
        }
    }
    LOG_DEBUG<<"finish processing "<<count<<" tokens in csv line";
    return true;
}

void set_guessing(std::string csv_entry, csv_info::entry_type& ci)
{
    boost::algorithm::trim(csv_entry);
    ci.total++;
    if (csv_entry.empty()) {
        ci.missing++;
    } else {
        if (is_date(csv_entry)) {
            LOG_DEBUG<<"date entry "<<csv_entry;
            ci.date_count += csv_entry;;
        } else if (is_number(csv_entry)) {
            LOG_DEBUG<<"numeric entry "<<csv_entry;
            ci.num_count += csv_entry;;
        } else {
            ci.str_count += csv_entry;;
        }
    }
}

}   // end of local namespace


///////////////////////////////////////////////////////////////////////////////

const char* column_nametype(column_type type)
{
    switch (type) {
        case column_type::TYPE_STRING:
            return details::column_typename<column_type::TYPE_STRING>::get();
        case column_type::TYPE_NUMBER:
            return details::column_typename<column_type::TYPE_NUMBER>::get();
        case column_type::TYPE_DATE:
            return details::column_typename<column_type::TYPE_DATE>::get();
        default:
            return "unknown";
    }
}

column_info::column_info() : 
    str_count(total), num_count(total), date_count(total)
{
}

#if 0
column_info& column_info::operator += (const column_info& other)
{
    str_count += other.str_count;
    num_count += other.num_count;
    date_count += other.date_count;
    missing += other.missing;
    total += other.total;
    return *this;
}

column_info operator + (const column_info& left, const column_info& right)
{
    column_info ret(left);
    ret += right;
    return ret;
}
#endif
bool operator == (const column_info& left, const column_info& right)
{
    return left.total == right.total && left.guess() == right.guess();
}


bool operator != (const column_info& left, const column_info& right)
{
    return !(left == right);
}

std::ostream& operator << (std::ostream& os, const column_info& ci)
{
    return os<<"index: "<<ci.index<<" missing: "<<ci.missing<<" out of total: "<<ci .total<<"\n"<<ci.str_count<<"\n"<<ci.num_count<<"\n"<<ci.date_count;
}

column_type column_info::guess() const
{
    if (str_count.count() > date_count.count()) {
        if (str_count.count() > num_count.count()) {
            return column_type::TYPE_STRING;
        } else {
            return column_type::TYPE_NUMBER;
        }
    } else {
        if (date_count.count() > num_count.count()) {
            return column_type::TYPE_DATE;
        } else {
            return column_type::TYPE_NUMBER;
        }
    }
}

const samples_collection::data_list& column_info::samples() const
{
    switch (guess()) {
        case column_type::TYPE_NUMBER:
            return num_count.all();
        case column_type::TYPE_STRING:
            return str_count.all();
        case column_type::TYPE_DATE:
            return date_count.all();
        default:
            return str_count.all();
    }
}

const char* guess_name(const column_info& column)
{
    switch (column.guess()) {
        case column_type::TYPE_NUMBER:
            return column.num_count.name();
        case column_type::TYPE_STRING:
            return column.str_count.name();
        case column_type::TYPE_DATE:
            return column.date_count.name();
        default:
            return "";
    }

}
///////////////////////////////////////////////////////////////////////////////

const csv_info::columns_list& csv_info::all() const
{
    return data;
}

bool csv_info::empty() const
{
    return data.empty();
}

csv_info::csv_info(int expect_c) : expect_count(expect_c)
{
}

std::size_t csv_info::columns_count() const
{
    return data.size();
}

csv_info::entry_type& csv_info::operator [] (std::size_t at)
{
    assert(at < columns_count());
    return data[at];
}


const csv_info::entry_type& csv_info::operator[] (std::size_t at) const
{
    if (at < columns_count()) {
        return data[at];
    } else {
        static const entry_type err = entry_type();
        return err;
    }
}

bool csv_info::process(std::istream& from, csv_info& to,
        char delim, char quote, std::size_t to_skip)
{
    csv::parser::source_type buffer{from};
    csv::parser parser{delim, quote};
    csv::rows_iterator<csv::parser> be{buffer, parser};
    csv::rows_iterator<csv::parser> e;
    if (to_skip == 0u && to.expected() < 0) {
        ++to_skip;
    }
    for (auto c = 0u; be != e && c < to_skip; ++c, ++be) {
        if (!handle_first(*be, to)) {    // this would be use only for the expected number of entries 
            LOG_ERROR<<"failed to process input line";
            return false;
        }
    }
    auto count = 0u;
    try {
        LOG_INFO<<"starting to process input data after skiping headers lines "<<to_skip;
        for (; be != e; ++be, ++count) {
            if (!process_line(*be, to)) {
                return false;
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR<<"error in processing input file "<<e.what();
        throw e;
    } catch (...) {
        if (count) {
            LOG_ERROR<<"encounter CSV parsing error after "<<count<<" lines";
            throw csv_parsing_error{};
        } else {
            LOG_ERROR<<"seems that we have an empty file";
            throw empty_input_file{};
        }
    }
    LOG_INFO<<"finish process "<<count<<" lines";
    // read the CSV data and process it
    return csv_info::good(to) && to[0].total > 0;
}

bool csv_info::good(const csv_info& ci)
{
    if (ci.empty()) {
        return false;
    } else {
        if (ci.expect_count == -1) {
            return true;
        } else {
            return (std::size_t)ci.expect_count == ci.columns_count();
        }
    }
}

int csv_info::expected() const
{
    return expect_count;
}

void csv_info::expected(int count)
{
    expect_count = count;
    data.resize((std::size_t)expect_count); // create list of empty entries
    for (auto i = 0; i < count; i++) {
        data[i].index = i;
    }
}

bool operator == (const csv_info& left, const csv_info& right)
{
    if (left.columns_count() == right.columns_count()) {
        for (auto i = 0u; i < left.columns_count(); ++i) {
            if (left[i] != right[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const csv_info& left, const csv_info& right)
{
    return !(left == right);
}

std::ostream& operator << (std::ostream& os, const csv_info& ci)
{
    if (csv_info::good(ci)) {
        os<<"columns: "<<ci.columns_count()<<"\n";
        for (auto i = 0u; i < ci.columns_count(); i++) {
            os<<"column ["<<i<<"]: "<<ci[i]<<"\n";
        }
        return os;
    } else {
        return os<<"invalid";
    }
}

samples_collection& samples_collection::operator += (const entry_type& new_entry)
{
    if (size() < max_size) {
        data.push_back(new_entry);
    }
    return *this;
}

bool samples_collection::empty() const
{
    return data.empty();
}

std::size_t samples_collection::size() const
{
    return data.size();
}

const samples_collection::data_list& samples_collection::all() const
{
    return data;
}

