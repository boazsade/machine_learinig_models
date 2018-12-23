#include "libs/ml_models/svm_types.h"
#include <boost/assign.hpp>
#include <map>

namespace mlmodels
{
namespace svm
{
type_t str2type(const std::string& from)
{
    static const std::map<std::string, type_t> mapper = 
        boost::assign::map_list_of("c_svc", C_SVC)
            ("nu_svc", NU_SVC)
            ("one_class", ONE_CLASS)
            ("eps_svr", EPS_SVR)
            ("nu_svr", NU_SVR);
    auto i = mapper.find(from);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid configuration for SVM " + from};
    }
    return i->second;
}

kernel_type str2kernel(const std::string& from)
{
    static const std::map<std::string, kernel_type> mapper = 
        boost::assign::map_list_of("custom", CUSTOM)
            ("linear", LINEAR)
            ("poly",POLY)
            ("rbf", RBF)
            ("sigmoid", SIGMOID)
            ("chi2", CHI2)
            ("inter", INTER);
    auto i = mapper.find(from);
    if (i == std::end(mapper)) {
        throw std::runtime_error{"invalid configuration for SVM " + from};
    }
    return i->second;
}
std::string to_string(kernel_type k)
{
    using namespace std::string_literals;
    switch (k) {
        case CUSTOM:
            return "custom"s;
        case LINEAR:
            return "linear"s;
        case POLY:
            return "poly"s;
        case RBF:    // default
            return "rbf"s;
        case SIGMOID:
            return "sigmoid"s;
        case CHI2:
            return "chi2"s;
        case INTER:
            return "inter"s;
    }
}

std::string to_string(type_t t)
{
    using namespace std::string_literals;
    switch (t) {
        case C_SVC:  // classifier
            return "c_svc"s;
        case NU_SVC:
            return "nu_svc"s;
        case ONE_CLASS:
            return "one_class"s;
        case EPS_SVR:
            return "eps_svr"s;
        case NU_SVR:  // regression
            return "nu_svr"s;
    }
}

}   // end of namespace svm
}   // end of namespace mlmodels

