#pragma once

#include <string>
#include <vector>

namespace csv
{

class row
{
public:
    using cell_type = std::string;
private:
    using row_store = std::vector<cell_type>;
public:


    using iterator = row_store::iterator;
    using const_iterator = row_store::const_iterator;


    int size() const;

    bool empty() const;

    const cell_type& operator [] (std::size_t off) const;

    const cell_type& at(std::size_t off) const;

    bool valid() const;

    int empty_cells() const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

protected:

    void add(const cell_type& c);    // at the end

private:
    row_store cells;
};

class mutable_row : public row
{

public:
    static constexpr int no_expected_columns = -1;

    explicit mutable_row(int expeced_column = no_expected_columns);

    mutable_row& operator += (const cell_type& new_cell);

    void reserve(int colmns);
};

}   // end of namespace csv

