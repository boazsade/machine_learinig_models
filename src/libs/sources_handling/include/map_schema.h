#pragma once
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>
#include <algorithm>
#include <string>
#include <map>
#include <iosfwd>

struct schemaid_iterator;
struct schema_index_iterator; 

struct schema_mapper
{
    using column_id = int;
    using id_type = std::string;
    using data_type = std::map<id_type, column_id>;

    static const constexpr column_id invalid = -1;

    schema_mapper() = default;

    template<typename It>
    schema_mapper(It be, It ed) {
        column_id inx = 0;
        auto& mp = columns;
        std::transform(be, ed, std::inserter(mp, mp.begin()), [&inx](const auto& id) {
                    return std::make_pair(id, inx++);
                }
        );
    }

    friend schemaid_iterator make_schemaid_iterator(const schema_mapper& from);
    friend schema_index_iterator make_index_iterator(const schema_mapper& from);

    column_id operator [] (const id_type& id) const;
    bool empty() const;
    std::size_t columns_num() const;


private:
    data_type columns;
};  

std::ostream& operator << (std::ostream& os, const schema_mapper& sm);

class schema_iterator_base
{
protected:
    ~schema_iterator_base() = default;
    schema_iterator_base() = default;
    schema_iterator_base(const schema_mapper::data_type& ref);
    void increment();
    bool equal(schema_iterator_base const& other) const;
public:
    struct null_ptr {};
    struct iter_range {
        using iterator = schema_mapper::data_type::const_iterator;

        iterator start;
        iterator end;

        iter_range() = default;
        iter_range(iterator b, iterator e);
    };

protected:
    friend class boost::iterator_core_access;
    using internal_rep = boost::variant<null_ptr, iter_range>;
    internal_rep ref_data;
};


struct schemaid_iterator : protected schema_iterator_base,
    public boost::iterator_facade<schemaid_iterator,
                           const schema_mapper::id_type,
                           boost::forward_traversal_tag>
{
    schemaid_iterator() = default;
    schemaid_iterator(const schema_mapper::data_type& ref);

private:
    friend class boost::iterator_core_access;
    bool equal(schemaid_iterator const& other) const;
    const schema_mapper::id_type& dereference() const;
public:
private:
};

schemaid_iterator make_schemaid_end_iterator();

struct schema_index_iterator : protected schema_iterator_base,
    public boost::iterator_facade<schema_index_iterator,
                           const schema_mapper::column_id,
                           boost::forward_traversal_tag,
                           schema_mapper::column_id>
{
    schema_index_iterator() = default;
    schema_index_iterator(const schema_mapper::data_type& ref);

private:
    friend class boost::iterator_core_access;
    bool equal(schema_index_iterator const& other) const;
    schema_mapper::column_id dereference() const;
};

schema_index_iterator make_end_index_iterator();

