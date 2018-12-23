#include "libs/experiments/kolmogorov_smironov_test.h"

namespace exprs
{
namespace ks
{

result_type accuracy_test(data_array test_data, data_array measured_data)
{
    if (test_data.empty() || measured_data.empty()) {
        static const std::string err{"both input array must have data in them - "};
        throw std::runtime_error{err + 
                    (test_data.empty() ? " test data is empty" : " measured data is empty")};
    } else if (test_data.size() != measured_data.size()) {
        static const std::string err{"data set and measured set must have same sizes: "};
        throw std::runtime_error{err + std::to_string(test_data.size()) + 
            " and " + std::to_string(measured_data.size())};
    } else {
        return utils::ks_test_accuracy(test_data, measured_data);
    }
}

}   // end of namespace ks
}   // end of namespace exprs
