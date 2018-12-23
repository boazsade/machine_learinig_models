#pragma once
#include "constfill_conf.h"
#include "fillinvalid_baseconf.h"
#include <iosfwd>
#include <string>

struct cubical_fix : fillinvalid_base
{
    using fillinvalid_base::fillinvalid_base;
};

std::string to_string(const cubical_fix&); 
std::ostream& operator << (std::ostream& os, const cubical_fix& kf);

struct linear_fix : fillinvalid_base
{
    using fillinvalid_base::fillinvalid_base;
};
std::ostream& operator << (std::ostream& os, const linear_fix& kf);
std::string to_string(const linear_fix&); 

struct kneighborhood_fix : fillinvalid_base
{
    using fillinvalid_base::fillinvalid_base;

    int n_count = 3;        // this is the default

    kneighborhood_fix() = default;
    kneighborhood_fix(const column_id& id, int count);
};
std::string to_string(const kneighborhood_fix&); 
std::ostream& operator << (std::ostream& os, const kneighborhood_fix& kf);

