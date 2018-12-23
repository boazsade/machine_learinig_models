#include "libs/ml_models/utilities.h"
//#include <map>
//#include <boost/range/algorithm/transform.hpp>

namespace mlmodels
{

classes_data transform(const class_data& source)
{
#if 0
    using mapping = std::map<class_data::value_type, int>;

    classes_data output(source.size(), class_data::value_type{});
    mapping mapper;
    int value = 0;
    boost::transform(source, output.begin(), [&mapper, &value] (auto val) {
            auto ret = mapper.insert(std::make_pair(val, value));
            return ret.second ? value++ : ret.first->second;
        }
    );

    return output;
#endif
    return classes_transform(std::begin(source), std::end(source));
}

}   // end of namespace mlmodels

