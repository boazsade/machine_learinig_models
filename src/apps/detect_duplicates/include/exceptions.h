#pragma once
#include <string>

struct no_such_path {
    no_such_path(const std::string& p) :
        msg(std::string("no such file or directory: ") + p)
    {
    }

    const std::string& what() const
    {
        return msg;
    }

    const std::string msg;
};

struct access_denied {
    access_denied(const std::string& f) :
        msg{std::string{"access denied to file "} + f}
    {
    }

    const std::string& what() const
    {
        return msg;
    }

    const std::string msg;
};

struct configuration_error {
    configuration_error(const std::string& e) : 
        msg{std::string{"error processing configuration: "} + e}
    {
    }

    const std::string& what() const
    {
        return msg;
    }

    const std::string msg;
};

