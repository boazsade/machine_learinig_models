#pragma once
#include "functional/functional.hpp"
#include "libs/rows_drops/drop_unwanted.h"
#include <iosfwd>

constexpr struct _fix_nothing {} fix_nothing = _fix_nothing{};  // default
std::string to_string(const _fix_nothing& du);

struct fix_missing
{
    using arg_type = drop_unwanted::result_type;
    using result_type = arg_type;
    using conf_type = poli_function<arg_type, result_type>;

    fix_missing();

    fix_missing(const conf_type& ct);
    fix_missing& operator = (fix_missing&&) = default;
    fix_missing(fix_missing&&) = default;
    fix_missing& operator = (const fix_missing&) = default;
    fix_missing(const fix_missing&) = default;
    
    result_type operator () (const arg_type& args) const;

    void print(std::ostream& os) const;
    const conf_type& config() const;
private:
    conf_type configuration;
};

fix_missing::result_type preform_action(
        const fix_missing& action, fix_missing::arg_type arg);
std::ostream& operator << (std::ostream& os, const fix_missing& du);
std::string to_string(const fix_missing& du);
std::ostream& operator << (std::ostream& os, _fix_nothing);

