#pragma once
namespace utils
{

template<typename T>
struct kalmogorov_smirnov_output {
    using result_type = T;

    result_type score = result_type{};
    result_type P_value = result_type{};

    constexpr kalmogorov_smirnov_output() = default;
    constexpr kalmogorov_smirnov_output(result_type d, result_type p) :
                score(d), P_value(p)
    {
    }
};

}   // end of namespace utils

