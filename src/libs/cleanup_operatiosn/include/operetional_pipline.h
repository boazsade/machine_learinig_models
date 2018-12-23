#pragma once
#include "fix_missing.h"
#include "no_normalized_conf.h"
#include "normalized_result.h"
#include "libs/sources_handling/source_data.h"
#include "functional/functional.hpp"
#include <iosfwd>

// note - we assuming that this is the last task that would be run
// if this is not the case, make sure to return correctly!!
struct normalized_data
{
    using arg_type = fix_missing::result_type;
    using result_type = normalized_result;
    using conf_type = poli_function<arg_type, result_type>;

    normalized_data();

    normalized_data(const conf_type& ct);
    normalized_data(normalized_data&&) = default;
    normalized_data& operator = (normalized_data&&) = default;
    normalized_data(const normalized_data&) = default;
    normalized_data& operator = (const normalized_data&) = default;

    result_type operator () (const arg_type& args) const;
    void print(std::ostream& os) const;
    const conf_type& config() const;
private:
    conf_type configuration;
};

normalized_data::result_type preform_action(
        const normalized_data& action, normalized_data::arg_type arg);
std::ostream& operator << (std::ostream& os, const normalized_data& du);
std::string to_string(const normalized_data& du);

