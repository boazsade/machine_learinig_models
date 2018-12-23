#pragma once
#include <iosfwd>
#include <array>

namespace mlmodels
{
namespace svm
{

struct sv_train_args
{
    double cost = 1;

    constexpr sv_train_args() = default;
    constexpr explicit sv_train_args(double c) : cost{c}
    {
    }
};
std::ostream& operator << (std::ostream& os, const sv_train_args& pa);

struct c_svm_train_args : sv_train_args
{
    static const constexpr int entries = 2;
    std::array<int, entries> labels;
    std::array<double, entries> weight;

    constexpr c_svm_train_args() : 
        labels{-1000, -1000}, weight{-1000, -1000}
    {
    }

    constexpr c_svm_train_args(double c, double cp, double cn) :
                    sv_train_args{c}, labels{1, -1},
                    weight{cp, cn}
    {
    }
};
std::ostream& operator <<(std::ostream& os, const c_svm_train_args& csvm);

struct nu_train_args
{
    double nu = 0.5;

    constexpr nu_train_args() = default;
    constexpr explicit nu_train_args(double n) : nu{n}
    {
    }
};
std::ostream& operator <<(std::ostream& os, const nu_train_args& csvm);


struct svr_args : nu_train_args, sv_train_args
{
    constexpr svr_args() = default;
    constexpr  svr_args(double n, double c) :
            nu_train_args{n}, sv_train_args{c}
    {
    }
};
std::ostream& operator <<(std::ostream& os, const svr_args& csvm);

struct epsilon_args : sv_train_args
{
    double epsilon = 0.1;
    constexpr epsilon_args() = default;
    constexpr epsilon_args(double c, double e) :
                    sv_train_args{c}, epsilon{e}
    {
    }
};
std::ostream& operator <<(std::ostream& os, const epsilon_args& ea);

}   // end of namesapce svm
}   // end of mlmodels

