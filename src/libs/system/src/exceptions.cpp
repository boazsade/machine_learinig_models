#include "libs/system//exceptions.h"

invalid_input::invalid_input(const std::string& p) :
        details{"invalid input: " + p}
{
}

const std::string& invalid_input::what() const
{
    return details;
}


invalid_conf::invalid_conf(const std::string& p) :
    details{"invalid configuration given: " + p}
{
}

const std::string& invalid_conf::what() const
{
    return details;
}

json_error::json_error(const std::string& p) :
    details{"JSON error: " + p}
{
}

const std::string& json_error::what() const
{
    return details;
}

invalid_path::invalid_path(const std::string& p) :
        err(std::string{"invalid path given at "} + p)
{
}

const std::string& invalid_path::what() const noexcept
{
        return err;
}

malformat_input::malformat_input(const std::string& at) :
    details{at + " contains inf <and|or> nan values. This may be the results of missing data or running the outliers phase when not required. If you think that the data is correct, do chose \"ignore\" in the Outlier phase and continue"}
{
}

const std::string& malformat_input::what() const
{
    return details;
}

norm_error::norm_error(const std::string& at) :
    details{at + " contains a single value. This might be the result of a missing data or not that this column is not relevant for this use case. We recommend you check your data, or add more data. If the data is correct and represents the big data, you can \"drop\" this column at the dataset phase"}
{
}

const std::string& norm_error::what() const
{
    return details;
}
