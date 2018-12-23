#pragma once
#include <iosfwd>

namespace mlmodels
{
namespace svm
{

struct rbf_args
{
    constexpr rbf_args() = default;
    constexpr explicit rbf_args(double g) : gamma{g}
    {
    }

    static constexpr double value(rbf_args a, std::size_t fc) {
        return is_default(a.gamma) ? default_best(fc) : a.gamma;
    }

    double gamma = default_val();

private:
    static constexpr double default_val() {
        return 0;
    }

    static constexpr bool is_default(double g) {
        return g == default_val();
    }

    static constexpr double default_best(std::size_t fc) {
        return 1.0/fc;
    }
};
std::ostream& operator << (std::ostream& os, const rbf_args& pa);

struct sig_args : rbf_args
{
     double coef0 = 0;

     constexpr sig_args() = default;
     constexpr sig_args(double g, double c) :
            rbf_args{g}, coef0{c}
     {
     }
};
std::ostream& operator << (std::ostream& os, const sig_args& pa);


struct poly_args : sig_args
{
    int degree = 3;

    constexpr poly_args() = default;
    constexpr poly_args(int d, double c, double g) :
            sig_args{g, c}, degree{d}
    {
    }
};
std::ostream& operator << (std::ostream& os, const poly_args& pa);

}   // end of namesapce svm
}   // end of mlmodels
