#pragma once
#include <boost/iterator/iterator_facade.hpp>
#include <iterator>

namespace csv
{

// iterate over a cells in a given row 
// this is the dereference value for rows_iterator
template<typename Parser, typename Source = typename Parser::source_type>
class cells_iterator : public boost::iterator_facade<cells_iterator<Parser>, typename Parser::token_type const, 
                                                std::input_iterator_tag,//boost::incrementable_traversal_tag,
                                                typename Parser::token_type const &>
{
public:

    using parser = Parser;
    using token_type = typename Parser::token_type;
    using source_type = Source;

    cells_iterator() = default;

    cells_iterator(cells_iterator&&) = default;
    cells_iterator& operator = (cells_iterator&&) = default;
    cells_iterator(const cells_iterator&) = default;
    cells_iterator& operator = (cells_iterator&) = default;

    explicit cells_iterator(source_type& s, parser& p) : input(&s)/*, state(STARTED)*/, the_parser(&p)
    {
        token = the_parser->get_token(*input);
    }

    bool valid() const
    {
        return /*state != DONE && state != INVALID &&*/ input && the_parser;
    }

    void reset(source_type& s, parser& p)
    {
        the_parser = &p;
        input = &s;
        token = the_parser->get_token(*input);
    }

private:
    friend class boost::iterator_core_access;

    void increment() 
    {       // we can increment only if we are in valid state
        if (valid()) {
            if (ended()) {
                input = nullptr;
                the_parser = nullptr;
                return;
            }

            token = the_parser->get_token(*input);
        }
    }

    bool ended() const
    {
        return the_parser->eol() || the_parser->eof() || the_parser->error();
    }

    bool equal(const cells_iterator& other) const
    {
        return valid() == other.valid();
    }

    const token_type& dereference() const
    {
        return token;
    }


private:
    source_type* input = nullptr;
    token_type token;
#if 0
    enum internal_state {
        IN_PROCESS, DONE, INVALID, STARTED
    };
    internal_state state = DONE;
#endif
    parser* the_parser = nullptr;
};

// iterate overa list of rows that we are reading from
// the input the return value for dereferencing is
// iterator of type cells_iterator
template<typename Parser, typename Source = typename Parser::source_type>
class rows_iterator : public boost::iterator_facade<rows_iterator<Parser>, 
                                             cells_iterator<Parser> const,
                                             boost::incrementable_traversal_tag,
                                             const cells_iterator<Parser>&
                      >
{
public:

    using parser = Parser;
    using result_type = cells_iterator<parser>;
    using source_type = Source;

    rows_iterator() = default;

    rows_iterator(rows_iterator&&) = default;
    rows_iterator& operator = (rows_iterator&&) = default;

    rows_iterator(const rows_iterator&) = default;
    //rows_iterator& operator = (const rows_iterator&) = default;

    explicit rows_iterator(source_type& s, parser& p) : cit(s, p), the_parser(&p), input(&s)
    {
    }

private:
    friend class boost::iterator_core_access;

    bool valid() const
    {
        return the_parser != nullptr;
    }

    void increment() 
    {
        if (valid()) {
            skip();
        } else {
            return;
        }
        if (the_parser && the_parser->eof()) {
            the_parser = nullptr;
        } else {
            cit.reset(*input, *the_parser);
        }
    }

    bool equal(const rows_iterator& other) const
    {
        return valid() == other.valid();
    }

    const result_type& dereference() const
    {
        return cit;
    } 

    void skip()
    {
        while (!the_parser->eol()) {
            ++cit;
            if (the_parser->error()) {
                the_parser = nullptr;
                break;
            }
        }
    }

private:
    result_type  cit;
    parser*      the_parser = nullptr;
    source_type* input = nullptr;
};

template<typename Parser, typename Source = typename Parser::source_type> inline
rows_iterator<Parser, Source> make_rows_iterator(Parser& p, Source& s)
{
    return rows_iterator<Parser, Source>{s, p};
}

template<typename Parser, typename Source = typename Parser::source_type> inline
rows_iterator<Parser, Source> make_rows_iterator_end(Parser&, Source&)
{
    return rows_iterator<Parser, Source>{};
}

}   // end of namespace csv

