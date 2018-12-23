#pragma once

#include <cstddef>
#include <string>
#include <stdexcept>

namespace csv
{

template<typename CharType>
struct string_buffer
{
    using string_type = std::basic_string<CharType>;
    using pointer = typename string_type::pointer;
    using const_pointer = typename string_type::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;


    string_buffer() = default;

    explicit string_buffer(string_type&& s) : 
        data(std::move(s)), start(data.empty() ? std::string::npos : 0), last(data.empty() ? std::string::npos : data.size())
    //start(data.empty() ? nullptr : &data[0]), last(data.empty() ? nullptr : &data[data.size()])
    {
    }

    bool assign(string_type&& s) 
    {
        std::swap(s, data);
        start = data.empty() ? string_type::npos : 0;
        last = data.empty() ? string_type::npos : data.size();
        return good();
    }   

    bool empty() const
    {
        return end() <= begin();
    }

    std::size_t size() const
    {
        return end() - begin();
    }

    iterator begin() 
    {
        return &data[start];
    }

    iterator end()
    {
        return &data[last];
    }

    const_iterator begin() const
    {
        return &data[start];
    }

    const_iterator end() const
    {
        return &data[last];
    }

    string_buffer& operator ++ ()
    {
        advance();
        return *this;
    }

    const_pointer get()
    {
        auto p = begin();
        advance();
        return p;
    }

    bool eof() const
    {
        return empty();
    }

    bool bad() const
    {
        return start == std::string::npos;
    }

    bool good() const
    {
        return !bad();
    }

private:

    void advance()
    {
        if (start < last) {
            ++start;
        } else {
            throw std::runtime_error{"invalid range"};
        }
    }

    string_type data;
    typename string_type::size_type start = string_type::npos;
    typename string_type::size_type last = string_type::npos;
    //pointer start = nullptr;
    //pointer last = nullptr;
};

template<typename Stream>
struct data_stream
{
    using stream_type = Stream;
    using char_type = typename Stream::char_type;

    data_stream() = default;
    data_stream(data_stream&&) = default;
    data_stream& operator = (data_stream&&) = default;
    data_stream(const data_stream&) = default;
    data_stream& operator = (const data_stream&) = default;

    explicit data_stream(stream_type& s) : in_stream(&s)
    {
    }

    data_stream& read(char_type* to, std::size_t max)
    {
        if (good()) {
            in_stream->get(to, max);
        } 
        return *this;
    }

    data_stream& read(std::basic_string<char_type>& into)
    {
        if (good()) {
            std::getline(*in_stream, into);
        }
        return *this;
    }

    data_stream& read(char_type* to, std::size_t max, char_type delim)
    {
        if (good()) {
            in_stream->get(to, max, delim);
        } 
        return *this;
    }

    data_stream& read(std::basic_string<char_type>& into, char_type delim)
    {
        if (good()) {
            std::getline(*in_stream, into, delim);
        }
        return *this;
    }



    bool good() const
    {
        return in_stream && in_stream->good();
    }

    bool error() const
    {
        return !good();
    }

    bool eof() const
    {
        return error() || in_stream->eof();
    }

private:
    stream_type* in_stream = nullptr;
};

}       // end of namespace csv

