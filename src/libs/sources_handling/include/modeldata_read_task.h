#include "row_data_input.h"
#include <boost/filesystem/path.hpp>
#include <string>
#include <iosfwd>

struct modeldata_read_base 
{
    using path_type = boost::filesystem::path;
    using arg_type = path_type;
    using result_type = classifier::read_result;
};

// read as is
constexpr struct modeldata_read_t : modeldata_read_base
{

    constexpr modeldata_read_t() = default;
} modeldata_read = modeldata_read_t{};

std::string to_string(modeldata_read_t );
std::ostream& operator << (std::ostream&, modeldata_read_t );

// it would generate the result as random ordered
constexpr struct  rand_modeldata_read_t : modeldata_read_base
{
    constexpr rand_modeldata_read_t() = default;
} rand_modeldata_read = rand_modeldata_read_t{};

std::string to_string(rand_modeldata_read_t );
std::ostream& operator << (std::ostream&, rand_modeldata_read_t );

modeldata_read_t::result_type
    preform_action(modeldata_read_t, const modeldata_read_t::arg_type& p);

rand_modeldata_read_t::result_type
    preform_action(rand_modeldata_read_t, const modeldata_read_t::arg_type& p);

