#include "math_helper_func.hpp"
#include "utils/algorithms.hpp"
#include <boost/range/algorithm/min_element.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iterator>
#include <vector>
#include <cmath>
#include <limits>


namespace
{

double calcualte_li(double xi, const std::vector<double>& xd, 
        const std::vector<double>& yd);

double calcualte_li(double xi, const std::vector<double>& xd, 
        const std::vector<double>& yd)
{
    if (xi <= xd[0]) {
        if (xd[1] == xd[0]) {
            return std::numeric_limits<double>::max();
        }
        auto t = (xi - xd[0]) / (xd[1] - xd[0]);
        return (1.0 - t) * yd[0] + t * yd[1];
    } else if (xd[xd.size()-1] <= xi) {
        if (xd[xd.size()-1] == xd[xd.size()-2]) {
            return std::numeric_limits<double>::max();
        }
        auto t = (xi - xd[xd.size()-2]) / (xd[xd.size()-1] - xd[xd.size()-2]);
        return (1.0 - t) * yd[xd.size()-2] + t * yd[xd.size()-1];
    } else {
        for (auto k = 1u; k < xd.size(); k++) {
            if (xd[k-1u] <= xi &&  xi <= xd[k] && (xd[k] != xd[k-1u])) {
                auto t = (xi - xd[k-1u]) / (xd[k] - xd[k-1u]);
                return (1.0 - t) * yd[k-1u] + t * yd[k];
            }
        }
    }
    return std::numeric_limits<double>::max();  // shold never get here
}


}   // end of local namespace

double linear_interpulation_1d(const std::vector<double>& x_vals,
        const std::vector<double>& y_vals, double x_cord)
{
    if (x_vals.size() == 1) {
        return y_vals[0];
    } else {
        return calcualte_li(x_cord, x_vals, y_vals);
    }
}

//****************************************************************************80
//
//  Purpose:
//
//    PWL_VALUE_1D evaluates the piecewise linear interpolant.
//
//  Discussion:
//
//    The piecewise linear interpolant L(ND,XD,YD)(X) is the piecewise
//    linear function which interpolates the data (XD(I),YD(I)) for I = 1
//    to ND.
//
//  Parameters:
//
//    Input, int ND, the number of data points.
//    ND must be at least 1.
//
//    Input, double XD[ND], the data points.
//
//    Input, double YD[ND], the data values.
//
//    Input, int NI, the number of interpolation points.
//
//    Input, double XI[NI], the interpolation points.
//
//    Output, double PWL_VALUE_1D[NI], the interpolated values.
//
std::vector<double> pwl_value_1d(std::vector<double> xd, 
        const std::vector<double>& yd,
        const std::vector<double>& xi)
{
    
    if (xd.size() == 1u) {
        return yd;
    }
    
    std::vector<double> yi;
    yi.reserve(xi.size());
    boost::transform(xi, std::back_inserter(yi), [&xd, &yd](auto x) {
                return calcualte_li(x, xd, yd);
            }
    );
    return yi;
}

//****************************************************************************80
//
//  Purpose:
//
//    NEAREST_INTERP_1D evaluates the nearest neighbor interpolant.
//
//  Discussion:
//
//    The nearest neighbor interpolant L(ND,XD,YD)(X) is the piecewise
//    constant function which interpolates the data (XD(I),YD(I)) for I = 1
//    to ND.
//
//  Licensing:
//
//  Parameters:
//
//    Input, int ND, the number of data points.
//    ND must be at least 1.
//
//    Input, double XD[ND], the data points.
//
//    Input, double YD[ND], the data values.
//
//    Input, int NI, the number of interpolation points.
//
//    Input, double XI[NI], the interpolation points.
//
//    Output, double NEAREST_INTERP_1D[NI], the interpolated values.
//

double nearset_interpolations(const std::vector<double>& x_vals,
        const std::vector<double>& y_vals, double x_cord)
{
    auto k = 0u;
    auto d = std::abs(x_cord - x_vals[k]);
    auto d2 = 0.0;
    for (auto j = 1u; j < x_vals.size(); ++j) {
        d2 = std::abs(x_cord - x_vals[j]);
        if (d2 <= d) {
            k = j;
            d = d2;
        }
    }
    return y_vals[k];
}

std::vector<double> nearest_interp_1d(std::vector<double> xd,
                                      std::vector<double> &yd,
                                      std::vector<double> &xi)

{
    std::vector<double> yi;
    yi.reserve(xi.size());
    boost::transform(xi, std::back_inserter(yi), [&xd, &yd] (auto x) {
                    return nearset_interpolations(xd, yd, x);
                }
    );
    return yi;
}


