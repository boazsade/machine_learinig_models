#pragma once
// we are using this as a place to implement the parsing itself
// basedon a state machine
#include "utils/state_machine.hpp"
#include <boost/mpl/vector/vector30.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace csv
{
namespace detail
{

struct csv_sm_parser : public state_machine<csv_sm_parser>
{
    //events
    struct reset_ev {}; // this is an internal state that we  would call once we want to start processing again
    struct read_any 
    {
        read_any() = default;

        constexpr explicit read_any(const char* ptr) : address(ptr)
        {
        }

        const char* get() const
        {
            return address;
        }

    private:
        const char* address = nullptr;
    };

    struct read_nl : read_any {
        using read_any::read_any;
    };
    struct read_delim : read_any {
        using read_any::read_any;
    };

    struct read_escape {};

    struct pending_entry {
        explicit pending_entry(const char* ch) : entry(ch)
        {
        }

        const char* get() const
        {
            return entry;
        }

    private:
        const char* entry;
    };

    struct read_eof {
        explicit read_eof(bool bad) : error(bad)
        {
        }

        bool error_state() const
        {
            return error;
        }

    private:
        bool error = false;
    };     // no input any more

    // end of events
    constexpr csv_sm_parser() = default;
    // functions that allow to read  current state
    bool record_ended() const
    {
        return pstate == EOR || eof();
    }

    bool eof() const
    {
        return pstate == END;
    }

    bool error() const
    {
        return pstate == ERROR;
    }

    bool good() const
    {
        return !error();
    }

    bool has_entry() const
    {
        return pstate == EOC || record_ended();
    }

    struct cell 
    {
        using pointer = const char*;
        using size_type = std::ptrdiff_t;

        constexpr cell() = default;
        cell(pointer s, pointer e) : start(s), last(e)
        {
        }

        cell(const cell&) = default;
        cell& operator = (const cell&) = default;
        cell(cell&&) = default;
        cell& operator = (cell&&) = default;

        size_type size() const
        {
            return last - start;
        }

        bool empty() const
        {
            return start == last;
        }

        bool valid() const
        {
            return start && last;
        }

        pointer begin() const
        {
            return start;
        }

        pointer end() const
        {
            return last;
        }

        friend std::string cast(const cell& from)
        {
            return std::string{from.begin(), from.end()};
        }

    private:
        pointer start = nullptr;
        pointer last = nullptr;
    };

    cell get_entry()
    {
        if (has_entry()) {
            cell c{cell_start, cell_end};
            return c;
        } else {
            return cell{};
        }
    }
            

private:
    using base_t = state_machine<csv_sm_parser>;
    friend class state_machine<csv_sm_parser>;
    // the state for the parser
    enum states {
        start_s,
        token_s,
        token_ends_s,
        escape_s,
        may_escape_s,
        may_end_escape_s,
        may_escape_escape_s,
        may_escape_escape_end_s,
        end_of_record_s,
        invalid_s,
        inital_state = start_s
    };

    // define the functions that handle the events
    //

    // functions that handle the events
    static void save_entry(csv_sm_parser& self, const read_any& ev)
    {
        if (self.cell_start) {
            self.cell_end++;
        } else {
            self.cell_start = ev.get();
            self.cell_end = self.cell_start;
        }
    }

    static void save_entry(csv_sm_parser& self, const read_nl& ev)
    {
        save_entry(self, static_cast<const read_any&>(ev));
    }

    static void save_entry(csv_sm_parser& self, const read_delim& ev)
    {
        save_entry(self, static_cast<const read_any&>(ev));
    }

    static void stop_processing(csv_sm_parser& self, const read_eof& event)
    {
        self.pstate = event.error_state() ? ERROR : END;
        if (self.pstate != ERROR) {
            ++self.cell_end;
        }
    }

    static void end_record(csv_sm_parser& self, const read_nl&)
    {
        self.pstate = EOR;
        if (self.cell_end) {
            ++self.cell_end;
        }
    }

    static void end_cell(csv_sm_parser& self, const read_delim&)
    {
        self.pstate = EOC;
        if (self.cell_end) {
            ++self.cell_end;
        }
    }

    static void read_escape_begin(csv_sm_parser& self, const read_escape&)
    {
        self.pstate = OTHER;
        if (self.cell_start) {  // we don't want ot keep those
            ++self.cell_start;
            ++self.cell_end;
        }
    }

    static void read_escape_end(csv_sm_parser& , const read_escape&)
    {
    }


    struct trasition_table : boost::mpl::vector< 
            //---------- current state -----------|-- event -------|-- next state ----|-- action ----------------------------|
             base_t::row<start_s                  ,  read_any      , token_s          , &csv_sm_parser::save_entry >,
             base_t::row<start_s                  ,  read_delim    , token_ends_s     , &csv_sm_parser::end_cell   >,
             base_t::row<start_s                  ,  read_nl       , end_of_record_s  , &csv_sm_parser::end_record >,
             base_t::row<start_s                  ,  read_eof      , invalid_s        , &csv_sm_parser::stop_processing >,
             base_t::row<token_s                  ,  read_any      , token_s          , &csv_sm_parser::save_entry >,
             base_t::row<token_s                  ,  read_nl       , end_of_record_s  , &csv_sm_parser::end_record >,
             base_t::row<token_s                  ,  read_delim    , token_ends_s     , &csv_sm_parser::end_cell   >,
             base_t::row<token_ends_s             ,  read_delim    , token_ends_s     , &csv_sm_parser::end_cell   >,
             base_t::row<token_ends_s             ,  read_nl       , end_of_record_s  , &csv_sm_parser::end_record >,
             base_t::row<end_of_record_s          ,  read_nl       , end_of_record_s  , &csv_sm_parser::end_record >,
             base_t::row<end_of_record_s          ,  read_any      , start_s          , &csv_sm_parser::save_entry >,
             base_t::row<end_of_record_s          ,  read_eof      , invalid_s        , &csv_sm_parser::stop_processing >,
             base_t::row<token_ends_s             ,  read_eof      , invalid_s        , &csv_sm_parser::stop_processing >,
             base_t::row<token_s                  ,  read_eof      , invalid_s        , &csv_sm_parser::stop_processing >,
             base_t::row<start_s                  ,  read_escape   , escape_s         , &csv_sm_parser::read_escape_begin>,
             base_t::row<escape_s                 ,  read_escape   , token_s          , &csv_sm_parser::read_escape_end>,
             base_t::row<escape_s                 ,  read_nl       , escape_s         , &csv_sm_parser::save_entry>,
             base_t::row<escape_s                 ,  read_delim    , escape_s         , &csv_sm_parser::save_entry>,
             base_t::row<escape_s                 ,  read_any      , escape_s         , &csv_sm_parser::save_entry>,
             base_t::row<escape_s                 ,  read_eof      , escape_s         , &csv_sm_parser::stop_processing>
    >
    {
    };

private:
    const char* cell_start = nullptr;
    const char* cell_end = nullptr;

    enum current_state {
        ERROR,
        END,
        EOR,
        EOC,
        OTHER
    };

    current_state pstate = OTHER;
};

/////////////////////////////////////////////////////////
//
struct csv_driver
{
    using parser = csv_sm_parser;
    using cell = parser::cell;

    constexpr csv_driver(char delim = ',', 
                         char esc = '"',
                         char esc_esc = '"'
            ) : delimiter(delim), escape(esc), escap_escap(esc_esc)
    {
    }

    csv_driver(csv_driver&&) = default;
    csv_driver& operator = (csv_driver&&) = default;
    csv_driver(const csv_driver&) = default;
    csv_driver& operator = (const csv_driver&) = default;

    template<typename Buffer>
    cell process(Buffer& input)
    {
        if (csv_parser.error() || input.bad()) {
            static constexpr cell err = cell{};
            return err;
        }
        csv_parser.process_event(parser::reset_ev{}); 
        while (csv_parser.good()) {
            if (input.eof()) {
                csv_parser.process_event(parser::read_eof{false});
                if (csv_parser.has_entry()) {
                    return csv_parser.get_entry();
                }
                break;
            } else if (input.bad()) {
                csv_parser.process_event(parser::read_eof{true});
                break;
            } else {
                if (csv_parser.has_entry()) {
                    return csv_parser.get_entry();
                } else {
                    if (!process_event(input.get())) {
                        break;
                    }
                }
            }
        }
        return cell{};
    }

    bool end_record() const
    {
        return csv_parser.record_ended();
    }

    bool eof() const
    {
        return csv_parser.eof();
    }

    bool error() const
    {
        return csv_parser.error();
    }

    bool good() const
    {
        return csv_parser.good();
    }

    bool has() const
    {
        return csv_parser.has_entry();
    }

    cell get_current()
    {
        return csv_parser.get_entry();
    }

private:
    bool process_event(const char* input)
    {
        if (*input == '\n') {
            csv_parser.process_event(parser::read_nl{input});
        } else if (*input == delimiter) {
            csv_parser.process_event(parser::read_delim{input});
        } else if (*input == escape) {
            csv_parser.process_event(parser::read_escape{});
        } else {
            csv_parser.process_event(parser::read_any{input});
        }
        return csv_parser.good();
    }


private:
    parser csv_parser;
    char delimiter;
    char escape;
    char escap_escap;
};

}   // end of namespace detail
}   // end of namesapce csv
