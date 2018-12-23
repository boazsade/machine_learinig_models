#pragma once
#include "general_types.h"
#include <map>
#include <algorithm>

namespace mlmodels
{

classes_data transform(const class_data& source);

template<typename Result = classes_data, typename It> inline
Result classes_transform(It from, It to)
{
    using mapping = std::map<class_data::value_type, int>;
    using output_t = typename Result::value_type;

    Result output(std::distance(from, to), classes_data::value_type{});
    int value = 1;
    mapping mapper;
    std::transform(from, to, std::begin(output), [&mapper, &value](auto val) {
                    auto ret = mapper.insert(std::make_pair(val, value));
                    auto r = ret.second ? value++ : ret.first->second;
                    return static_cast<output_t>(r);
                }
            );
    return output;
}

template<typename It, typename DT = value_type> inline
std::map<DT, int> make_transformer(It from, It to)
{
    using result_type = std::map<DT, int>;
    result_type output;
    int value = 1;
    std::for_each(from, to, [&output, &value](auto v) {
            auto r = output.insert(std::make_pair(v, value));
            if (r.second) {
                ++value;
            }
        }
    );
    return output;
}

}   // end of namespace mlmodels

