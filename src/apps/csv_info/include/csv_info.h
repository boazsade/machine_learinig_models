#pragma once
#include <cstdint>
#include <vector>
#include <iostream>

enum class column_type : short {
    TYPE_STRING = 0,
    TYPE_NUMBER,
    TYPE_DATE,
    UKNOWN_TYPE
};

namespace details {
    template<column_type>
    struct column_typename;

    template<>
    struct column_typename<column_type::TYPE_DATE> {
        static const char* get() {
            return "DATE";
        }
    };

    template<>
    struct column_typename<column_type::TYPE_NUMBER> {
        static const char* get() {
            return "NUMBER";
        }
    };

    template<>
    struct column_typename<column_type::TYPE_STRING> {
        static const char* get() {
            return "STRING";
        }
    };

    template<>
    struct column_typename<column_type::UKNOWN_TYPE> {
        static const char* get() {
            return "UNKOWN";
        }
    };
}   // end of namespace details

const char* column_nametype(column_type type);

template<column_type Type>
struct type_counter {
    using value_type = std::size_t;
    value_type value = 0;
    value_type totals;

    explicit type_counter(value_type t) : totals(t)
    {
    }

    type_counter& operator ++ () 
    {
        ++value;
        ++totals;
        return *this;
    }

    type_counter operator ++ (int)
    {
        type_counter t{*this};
        ++value;
        ++totals;
        return t;
    }

    type_counter& operator += (std::size_t by)
    {
        value += by;
        totals += by;
        return *this;
    }

    type_counter& operator += (const type_counter& other) 
    {
        value += other.value;
        return *this;
    }
};

template<column_type Type> inline 
type_counter<Type> operator + (const type_counter<Type>& tc, std::size_t v)
{
    type_counter<Type> result{tc};
    result += v;
    return result;
}

template<column_type Type> inline 
type_counter<Type> operator + (const type_counter<Type>& tc, const type_counter<Type>& right)
{
    type_counter<Type> result{tc};
    result += right;
    return result;
}

template<column_type Type> inline 
std::ostream& operator << (std::ostream& os, const  type_counter<Type>& tc) {
    return os<<"type: "<<details::column_typename<Type>::get()<<": "<<tc.value<<", out of "<<tc.totals;
}

struct samples_collection
{
    using data_list = std::vector<std::string>;
    using entry_type = data_list::value_type;

    static const std::size_t max_size = 1000;

    samples_collection& operator += (const entry_type& new_entry);
    
    bool empty() const;

    std::size_t size() const;

    const data_list& all() const;

private:
    data_list data;
};

template<column_type Type>
struct counters_and_samples : public samples_collection
{
    using counter_type = type_counter<Type>;

    explicit counters_and_samples(typename counter_type::value_type& t) : counter(t)
    {
    }

    typename counter_type::value_type count() const
    {
        return counter.value;
    }

    typename counter_type::value_type total() const
    {
        return counter.totals;
    }

    const char* name() const
    {
        return details::column_typename<Type>::get();
    }

    counters_and_samples& operator += (const samples_collection::entry_type& et)
    {
        ++counter;
        static_cast<samples_collection&>(*this) += et;
        return *this;
    }

private:
    counter_type counter;
};

template<column_type Type> inline
std::ostream& operator << (std::ostream& os, const counters_and_samples<Type>& cs)
{
    return os<<"counter "<<cs.name()<<" cuont: "<<cs.count()<<" out of "<<cs.total()<<"with samples: "<<cs.size()<<" samples";
}

using string_counter = type_counter<column_type::TYPE_STRING>;
using number_counter = type_counter<column_type::TYPE_NUMBER>;
using date_counter = type_counter<column_type::TYPE_DATE>;

using str_count_samples = counters_and_samples<column_type::TYPE_STRING>;
using num_count_samples = counters_and_samples<column_type::TYPE_NUMBER>;
using data_count_samples = counters_and_samples<column_type::TYPE_DATE>;


struct column_info {
    //using column_samples = std::vector<std::string>;

    std::size_t total = 0;
    std::size_t missing = 0;
    int index = -1;
    
    str_count_samples str_count;
    num_count_samples num_count;
    data_count_samples   date_count;

    //column_samples samples;

    column_info();

    //column_info& operator += (const column_info& other);

    column_type guess() const;

    const samples_collection::data_list& samples() const;
};

//column_info operator + (const column_info& left, const column_info& right);
bool operator == (const column_info& left, const column_info& right);
bool operator != (const column_info& left, const column_info& right);
const char* guess_name(const column_info& column);
std::ostream& operator << (std::ostream& os, const column_info& ci);


struct csv_info {
    using columns_list = std::vector<column_info>;    
    using entry_type = columns_list::value_type;

    explicit csv_info(int expect_c = -1);

    bool empty() const;

    std::size_t columns_count() const;

    const entry_type& operator [] (std::size_t at) const;
    entry_type& operator [] (std::size_t at);

    static bool process(std::istream& from, csv_info& to,
            char delim, char quote, std::size_t to_skip);

    static bool good(const csv_info& ci);

    int expected() const;

    void expected(int count);

    const columns_list& all() const;
private:
    columns_list data;
    int expect_count = -1;
};

bool operator == (const csv_info& left, const csv_info& right);
bool operator != (const csv_info& left, const csv_info& right);
std::ostream& operator << (std::ostream& os, const csv_info& ci);

