#include "libs/csv_parser/csv_parser.h"
#include <iostream>
#include <algorithm>


namespace csv
{

namespace
{
    parser::state get_current_state(parser::state current)
    {
        switch (current) {
            case parser::INIT:
            case parser::IN_TOKEN:
            case parser::END_ESCAPE_IN_TOKEN:
                return parser::MAY_ESCAPE;
            case parser::END_LINE:
            case parser::END_INPUT:
            case parser::ERROR:
                return current;
            case parser::IN_ESCAPE:
                return parser::MAY_ESCAPE_END;
            case parser::MAY_ESCAPE_END:
                return parser::IN_TOKEN;
            default:
                return current;
        }
    }
}


bool parser::internal_process2(char in)
{
    // a more complex case - we have the same char for escape and to escape the escape
   if (in ==  delimiter) { // we found a delimiter, but are we 
        if (current_state != parser::IN_ESCAPE) {
            current_state = parser::END_TOKEN;
            return true;    // we have found what we are looking for - the end of token
        } else {
            token += in;
        }

    } else if ( in ==  escape_char) {
        current_state = get_current_state(current_state);
        if (current_state != ESCAPE_OF_ESCAPE) {
            token += in;
        }
    } else if (in == '\n') {
        if (current_state != parser::IN_ESCAPE) {
            current_state = parser::END_LINE;
            return true;
        } else {
            token += in;
        }
    } else {
        if (current_state == parser::ESCAPE_OF_ESCAPE) {
            current_state = parser::IN_TOKEN;
        } else {
            token += in;
        }
    }
    return false;

}

bool parser::internal_process(char in)
{
   if (in ==  delimiter) { // we found a delimiter, but are we 
        if (current_state != parser::IN_ESCAPE) {
            current_state = parser::END_TOKEN;
            return true;    // we have found what we are looking for - the end of token
        } else {
            token += in;
        }

    } else if ( in ==  escape_char) {
        if (current_state == parser::IN_ESCAPE) {
            current_state = parser::END_ESCAPE_IN_TOKEN;
        } else if (current_state != ESCAPE_OF_ESCAPE) {
            current_state = IN_ESCAPE;
        } else {
            token += in;
        }
    } else if (in == escape_escape_char) {
        if (current_state != parser::IN_ESCAPE) {
            current_state = parser::ESCAPE_OF_ESCAPE;
        } else {
            token += in;
        }
    } else if (in == '\n') {
        if (current_state != parser::IN_ESCAPE) {
            current_state = parser::END_LINE;
            return true;
        } else {
            token += in;
        }
    } else {
        if (current_state == parser::ESCAPE_OF_ESCAPE) {
            current_state = parser::IN_TOKEN;
        } else {
            token += in;
        }
    }
    return false;

}


parser::parser(char delim, char escape, char escape_escape) : delimiter(delim), escape_char(escape), escape_escape_char(escape_escape)
{
}

bool parser::error() const
{
    return (current_state == ERROR);
}

bool parser::good() const
{
    return !error();
}

bool parser::has_token() const
{
    return good() && (current_state == END_TOKEN || current_state == END_LINE || current_state == END_INPUT);
}

bool parser::eol() const
{
    return good() && (current_state == END_LINE || current_state == END_INPUT);
}

bool parser::eof() const
{
    return (current_state == END_INPUT);
}

bool parser::get(std::string& new_token)
{
    if (good()) {
        std::swap(token, new_token);
        token.clear();
        return !new_token.empty();
    }
    current_state = INIT;
    return false;
}

int parser::process(source_type& from)
{
    bool ret = false;
    if (error()) {
        return -1;
    }
    
    int count = 0;
    while (from) {
        char in = from.get();
        if ((int)in > 0) {
            ++count;
            ret = internal_process(in);
            if (ret) {
                break;
            }
        }
    }
    if (from.eof()) {
        if (ret || current_state != ERROR) {
            current_state = END_INPUT;
        } else {
            current_state = ERROR;
        }
    }
    return current_state == ERROR ? -1 : count;
}

std::string parser::get_token()
{
    std::string res;
    if (get(res)) {
        return res;
    } else {
        static const std::string error = std::string{};
        return error;
    }
}

std::string parser::get_token(source_type& from)
{
    process(from);
    return get_token();
}

}   // end of namespace csv

