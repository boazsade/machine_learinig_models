#pragma once
#include "functional/functional.hpp"
#include "libs/sources_handling/source_data.h"
#include <iosfwd>

constexpr struct _dont_drop{} dont_drop = _dont_drop{};
std::string to_string(const _dont_drop&);

struct drop_unwanted
{
    using arg_type = source_data;
    using result_type = source_data;
    using conf_type = poli_function<arg_type, result_type>;

    drop_unwanted();
    drop_unwanted(const conf_type& ct);
    drop_unwanted(drop_unwanted&&) = default;
    drop_unwanted(const drop_unwanted&) = default;
    drop_unwanted& operator = (drop_unwanted&&) = default;
    drop_unwanted& operator = (const drop_unwanted&) = default;
    drop_unwanted& operator = (const conf_type&);

    result_type operator () (const arg_type& args) const;

    void print(std::ostream& os) const;

    const conf_type& config() const;

private:
    conf_type configuration;
};

drop_unwanted::result_type preform_action(
        const drop_unwanted& action, drop_unwanted::arg_type arg);
std::ostream& operator << (std::ostream& os, const drop_unwanted& du);
std::ostream& operator << (std::ostream& os, const _dont_drop& du);
std::string to_string(const drop_unwanted& du);


