#include "libs/sources_handling/map_schema.h"
#include <iostream>

namespace
{
struct deref_id :
        boost::static_visitor<const schema_mapper::id_type&>
{
    const schema_mapper::id_type& operator () (schema_iterator_base::null_ptr) const {
        using namespace std::string_literals;
        static const std::string empty = ""s;
        return empty;
    }
    const schema_mapper::id_type& operator ()(const schema_iterator_base::iter_range& r) const {
        if (r.start != r.end) {
            return r.start->first;
        } else {
            return this->operator()(schema_iterator_base::null_ptr{});
        }
    }
};

struct deref_column :
    boost::static_visitor<schema_mapper::column_id>
{
    schema_mapper::column_id operator () (schema_iterator_base::null_ptr) const {
        return schema_mapper::invalid;
    }

    schema_mapper::column_id operator () (const schema_iterator_base::iter_range& r) const {
        if (r.start != r.end) {
            return r.start->second;
        } else {
            return this->operator()(schema_iterator_base::null_ptr{});
        }
    }
};

struct incr_itertion :
    boost::static_visitor<> 
{
    void operator () (schema_iterator_base::null_ptr) const {
    }

    void operator () (schema_iterator_base::iter_range& r) const {
        if (r.start != r.end) {
            ++r.start;
        }
    }
};

struct compare_equal :
    boost::static_visitor<bool>
{
    bool operator () (schema_iterator_base::null_ptr, schema_iterator_base::null_ptr) const
    {
        return true;
    }

    bool operator () (const schema_iterator_base::iter_range& l, 
            const schema_iterator_base::iter_range& r) const
    {
        if (l.start == l.end && r.start == r.end) {
            return true;
        } else if (l.start != l.end && r.start != r.end) {
            return true;
        } else {
            return false;
        }
    }

    bool operator () (const schema_iterator_base::iter_range& l,
            schema_iterator_base::null_ptr) const
    {
        return l.start == l.end;
    }

    bool operator () (schema_iterator_base::null_ptr,
            const schema_iterator_base::iter_range& r) const
    {
        return r.start == r.end;
    }

};

}   // end of local namespace

schema_mapper::column_id schema_mapper::operator[] (const id_type& id) const
{
    auto i = columns.find(id);
    return i == columns.end() ? invalid : i->second;
}
bool schema_mapper::empty() const
{
    return columns.empty();
}

std::size_t schema_mapper::columns_num() const
{
    return columns.size();
}

schema_iterator_base::iter_range::iter_range(iterator b, iterator e) :
        start{b}, end{e}
{

}

schema_iterator_base::schema_iterator_base(const schema_mapper::data_type& mp) :
        ref_data{iter_range{mp.begin(), mp.end()}}
{
}

void schema_iterator_base::increment()
{
    boost::apply_visitor(incr_itertion{}, ref_data);
}

bool schema_iterator_base::equal(const schema_iterator_base& other) const
{
    return boost::apply_visitor(compare_equal{}, ref_data, other.ref_data);
}

const schema_mapper::id_type& schemaid_iterator::dereference() const
{
    return boost::apply_visitor(deref_id{}, ref_data);
}


schemaid_iterator::schemaid_iterator(const schema_mapper::data_type& ref) :
    schema_iterator_base{ref}
{
}

bool schemaid_iterator::equal(const schemaid_iterator& other) const
{
    return schema_iterator_base::equal(static_cast<const schema_iterator_base&>(other));
}

schema_index_iterator::schema_index_iterator(const schema_mapper::data_type& ref) :
            schema_iterator_base{ref}            
{
}

bool schema_index_iterator::equal(schema_index_iterator const& other) const
{
    return schema_iterator_base::equal(static_cast<const schema_iterator_base&>(other));
}

schema_mapper::column_id schema_index_iterator::dereference() const
{
    return boost::apply_visitor(deref_column{}, ref_data);
}

schema_index_iterator make_end_index_iterator()
{
    return schema_index_iterator{};
}

schemaid_iterator make_schemaid_end_iterator()
{
    return schemaid_iterator{};
}

schemaid_iterator make_schemaid_iterator(const schema_mapper& from)
{
    return from.empty() ? make_schemaid_end_iterator() : schemaid_iterator{from.columns};
}

schema_index_iterator make_index_iterator(const schema_mapper& from)
{
    return from.empty() ? make_end_index_iterator() : schema_index_iterator{from.columns};
}

std::ostream& operator << (std::ostream& os, const schema_mapper& sm)
{
    if (sm.empty()) {
        return os<<"empty";
    } else {
        const auto end = make_schemaid_end_iterator(); 
        for (auto i = make_schemaid_iterator(sm); i != end; ++i) {
            os<<"["<<*i<<"] = "<<sm[*i]<<"\n";
        }
    }
    return os;
}
