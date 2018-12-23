#include "utils/math_utils.h"
#include "math_helper_func.hpp"
#include "utils/missing_values.hpp"
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/special_functions/beta.hpp>
//#include <opencv2/opencv.hpp>
#include <algorithm>
#include <numeric>
#include <cmath>
#if 0
#include <iostream>
#include <iterator>
#include <algorithm>
#endif

namespace utils
{

namespace
{

double calctinv(double p, double v);
double tinv(double p, double v);
double zcritical(double alpha, double n);
std::size_t calcualte_max(std::vector<double> range, double meanval);
double find_outlier(std::vector<double>& input, double alpha);

double calctinv(double p, double v)
{
    //For large d.f., use Abramowitz & Stegun formula 26.7.5
    double xn=boost::math::detail::find_inverse_s(p, (1 - p));
    double df = v;
    double xn3 = std::pow(xn, 3);
    double xn5 = std::pow(xn, 5);
    double xn7 = std::pow(xn, 7);
    double xn9 = std::pow(xn, 9);


    double x = xn + (xn3+xn)/(4*df) + 
        (5*xn5 + 16*xn3 + 3*xn)/(96*df*df) +
        (3*xn7 + 19*xn5 + 17*xn3 - 15*xn)/(384*df*df*df) +
        (79*xn9 + 776*xn7 + 1482*xn5 - 1920*xn3 - 945*xn)/(92160*df*df*df*df);
    return x;
}

double tinv(double p, double v)
{
    using boost::math::beta_distribution;
    using boost::math::sign;
    //   TINV   Inverse of Student's T cumulative distribution function (cdf).
    //   X=TINV(P,V) returns the inverse of Student's T cdf with V degrees
    //   of freedom, at the values in P.
    //   References:
    //      [1]  M. Abramowitz and I. A. Stegun, "Handbook of Mathematical
    //      Functions", Government Printing Office, 1964, 26.6.2

    //   Copyright 1993-2014 The MathWorks, Inc.
    double x = 0;
    bool k0 = (0 < p && p < 1) && (v > 0);

    // Invert the Cauchy distribution explicitly
    int k = (k0 && (v == 1));
    if (k) {
        x = tan(M_PI * (p - 0.5));
    }

    // For small d.f., call betaincinv which uses Newton's method
    k = (k0 && (v < 1000) && (v != 1));
    if (k) {
        double q = p - .5;
        double df = v;
        int t = (std::abs(q) < .25);
        double z = 0;
        double oneminusz=0;

        if (t) {
            // for z close to 1, compute 1-z directly to avoid roundoff
            beta_distribution<> m1(0.5, df/2);
            oneminusz = quantile(m1, 2.0 * std::abs(q));
            z = 1 - oneminusz;

        } else {
            beta_distribution<> m(df/2, 0.5);
            z = quantile(m, 1 - 2.0 * std::abs(q));
            oneminusz = 1 - z;
        }
        if (z == 0.0) {
            return z;   // an error
        }
        x = sign(q) * sqrt(df * (oneminusz/z));
    }

    // For large d.f., use Abramowitz & Stegun formula 26.7.5
    k = (k0 && (v >= 1000));
    if (k) {
        x= calctinv( p, v );
    }
    return x;
}


double zcritical(double alpha, double n)
{
    //Computes the critical z value for rejecting outliers (GRUBBS TEST)
    double tcrit = tinv(alpha/(2*n), n-2);
    if (tcrit == 0.0) {
        return 0.0;     // we have an issue with the value - make it an outlier
    }
    double zcrit = (n - 1)/sqrt(n) * (sqrt(tcrit * tcrit/(n-2 + tcrit * tcrit)));
    return zcrit;
}

std::size_t calcualte_max(std::vector<double> range, double meanval)
{
    std::transform(std::begin(range), std::end(range), std::begin(range),
            [meanval](double val) {
                val -= meanval;
                return std::abs(val);
            }
        );
    auto i =  std::max_element(std::begin(range), std::end(range));
    return std::distance(std::begin(range), i);
}

double find_outlier(std::vector<double>& input, double alpha)
{
    if (input.empty()) {
        return missing_value<double>();
    }
    auto mean_std = mean_standard_dev<double>(std::begin(input), std::end(input), input.size());
    if (mean_std.standard_dev == 0.0) { // all values are the same, there are no outliers for sure..
        return missing_value<double>();
    }
    auto max_index = calcualte_max(input, mean_std.mean);
    auto maxval = input[max_index];
    auto tn = std::abs((maxval - mean_std.mean)/mean_std.standard_dev);
    auto critical = zcritical(alpha, input.size());
    if (tn > critical) {
        input.erase(std::begin(input) + max_index);
        return maxval;
    }
    return missing_value<double>();
}

}   // end of local namespace

double round_by(double from, unsigned int presition)
{
    static const double factors[] = {
        std::pow(10, 0),
        std::pow(10, 1),
        std::pow(10, 2),
        std::pow(10, 3),
        std::pow(10, 4),
        std::pow(10, 5),
        std::pow(10, 6),
        std::pow(10, 7),
        std::pow(10, 8),
        std::pow(10, 9),
        std::pow(10, 10),
        std::pow(10, 11),
        std::pow(10, 12),
        std::pow(10, 13),
        std::pow(10, 14),
        std::pow(10, 15),
        std::pow(10, 16),
        std::pow(10, 17),
        std::pow(10, 18),
        std::pow(10, 19),
        std::pow(10, 20),
        std::pow(10, 21),
        std::pow(10, 22),
        std::pow(10, 23)
    };
    static const std::size_t SIZE = sizeof (factors)/sizeof(factors[0]);

    if (presition == SIZE) {
        return from;
    }

    from *= factors[presition];
    from = std::floor(from + 0.5);
    from /= factors[presition];
    return from;
}
//typical alpha =0.05;0.025
std::vector<double>
grubbstest(std::vector<double> input, double alpha)
{
    using value_type = double;
    using result_type = std::vector<value_type>;

    auto i = std::remove_if(std::begin(input),
            std::end(input), [](auto d) { return std::isinf(d); }
        );
    input.erase(i, std::end(input));
    result_type result; 
    auto outlier = find_outlier(input, alpha);
    while (!missing_value(outlier)) {
        result.push_back(outlier);
        outlier = find_outlier(input, alpha);
    }
                
    return result;
    
}
}   // end of namespace utils

