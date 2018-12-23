#pragma once
#include <string>
#include <exception>
// list of exceptions that can be use
// to report for outstanding cases when running the
// experiments
struct invalid_input
{
    explicit invalid_input(const std::string& p);

    const std::string& what() const;

    const std::string details;
};

struct invalid_conf
{
    explicit invalid_conf(const std::string& p);

    const std::string& what() const;

    const std::string details;
};

struct json_error 
{
    explicit json_error(const std::string& p);

    const std::string& what() const;

    const std::string details;
};

struct invalid_path
{
    explicit invalid_path(const std::string& p);
    
    const std::string& what() const noexcept ;
private:
    const std::string err;
};

struct malformat_input 
{
    explicit malformat_input(const std::string& w);

    const std::string& what() const;

    const std::string details;
};

struct norm_error 
{
    explicit norm_error(const std::string& at);

    const std::string& what() const;

    const std::string details;
};
