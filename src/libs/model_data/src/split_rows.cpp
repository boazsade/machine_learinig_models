#include "libs/model_data/split_rows.h"

namespace classifier
{
unsigned int spliter_at(unsigned int precent, std::size_t size)
{
    auto cut = (100u - precent);          // for test we are taking the lower part of 100%
    auto s = size * cut;    // we would need to turn this to percent
    return s /= 100u;
}

unsigned int spliter_negate(unsigned int precent, std::size_t size)
{
    return size - spliter_at(precent, size);
}

}       // end of namespace classifier

