#include "utils/cubic_iterpolation.h"
#include <limits>
#include <cmath>
#include <stdexcept>
#include <string>
    

namespace cubic
{

namespace
{
///////////////////////////////////////////////////////////////////////////////
//	function: void spline(double *x, double *y, int n, double yp1, double ypn,
//							double *y2)
//	Compute the second derivative values for the interpolating function.
//	Input:	*x		-	tabulated x values
//			*y		-	tabulated y=f(x) values
//			n		-	size of the arrays
//			yp1		-	first derivative at point 1
//			ypn		-	first derivative at point n
//	Output:	*y2		-	array of second derivatives
//
//	Slightly modified code from Numerical Recipes in C, Section 3.3
///////////////////////////////////////////////////////////////////////////////

values_type  spline(const coordinates_type& x, const values_type& y);

//*****************************************************************************
//	function: int splint(double *xa, double *ya, double *y2, int n,
//							double x, double *y)
//	Compute the second derivative values for the interpolating function.
//	Input:	*xa		-	tabulated x values
//		*ya		-	tabulated y=f(x) values
//		*y2		-	output from spline()
//		n		-	size of the arrays
//		x		-	input x value
//	Output:	*y		-	interpolated y value
//	Returns:		true = success
//				false = error
//
//	Slightly modified code from Numerical Recipes in C, Section 3.3
//*****************************************************************************
double splint(const coordinates_type& xa, const values_type& ya,
           const values_type& y2, double x);


values_type spline(const coordinates_type& x, 
        const values_type& y)
{
    static const values_type error_case = values_type{};

    auto xsize = x.size();
    if ((x[1] == x[0]) || (x[xsize-1] == x[xsize-2])) {
        return error_case;
    }
    double qn = 0.0, un = 0.0;
    values_type  y2(x.size(), 0);
    values_type u(x.size(), 0);
    //	Compute first derivatives at point 1 and point n.
    double yp1 = (y[1] - y[0]) / (x[1] - x[0]);
    double ypn = (y[xsize-1] - y[xsize-2]) / (x[xsize-1] - x[xsize-2]);
    //	Spline computation.
    if (yp1 > std::numeric_limits<double>::max()) {
        y2[0] = u[0] = 0.0;
    } else {
        y2[0] = -0.5;
        u[0] = ( 3.0 / (x[1] - x[0])) * ((y[1] - y[0]) / (x[1] - x[0]) - yp1);
    }
        
    for (auto i = 1u; i <= (xsize - 2u); ++i) {
        if ((x[i+1u] == x[i-1u])) {
            return error_case;
        }
        double sig = (x[i] - x[i-1u]) / (x[i+1u] - x[i-1u]);
        double p = sig * y2[i-1u] + 2.0;
        if (p == 0.0 || sig == 0.0) {
            return error_case;
        }
        y2[i] = (sig - 1.0) / p;
        u[i] = (y[i+1u] - y[i] ) / (x[i+1u] - x[i]) - (y[i] - y[i-1u]) / (x[i] - x[i-1u]);
        u[i] = (6.0 * u[i] / (x[i+1u] - x[i-1u]) - sig * u[i-1u]) / p;
    }
        
    if (ypn < std::numeric_limits<double>::max()) {
        qn = 0.5;
        un = (3.0 / (x[xsize-1] - x[xsize-2])) * (ypn - (y[xsize-1] - y[xsize-2]) / (x[xsize-1] - x[xsize-2]));
    }
        
    y2[xsize-1] = (un - qn * u[xsize-2]) / (qn * y2[xsize-2] + 1.0);
        
    for (int k = (int)(xsize - 2u); k >= 0; k--) {
        y2[k] = y2[k] * y2[k+1] + u[k];
    }
    return y2;
}

double splint(const coordinates_type& xa,
            const values_type& ya,
            const values_type& y2,
            double x)
{
    if (y2.empty()) {
        return std::numeric_limits<double>::max();
    }
    int k = 0;
    int  const n = (int)xa.size();
    int klo = 0;
    int khi = n;
    //binary search
    while (khi - klo > 1) {
        k = (khi + klo) >> 1;
        if (xa[k] > x){
            khi = k;
        } else {
            klo = k;
        }
    }
    
    double h = xa[khi] - xa[klo];
    if (h == 0.0) {
        return std::numeric_limits<double>::max();;
    }
    double a = (xa[khi] - x) / h;
    double b = (x - xa[klo]) / h;
    return a * ya[klo] + b * ya[khi] + ((std::pow(a, 3) - a) * y2[klo] + (std::pow(b, 3) - b) * y2[khi]) * (h * h)/6.0;
}

}   // end of local namespace 

spline_data::spline_data(coordinates_type xcords,
                        values_type fx) :
            x_coordinate{std::move(xcords)},
            x_values{std::move(fx)},
            second_deriv_vals{spline(x_coordinate, x_values)}
{
    if (second_deriv_vals.empty()) {
        throw std::runtime_error{"failed to calculate second drivative for input on x axis of size " + 
                                std::to_string(x_coordinate.size()) + " and values size of " + std::to_string(x_values.size())
                            };
    }
}

values_type::value_type
    interpole(const spline_data& data, coordinates_type::value_type at)
{
    return splint(data.x_coordinate, data.x_values, data.second_deriv_vals, at);
}

}   // end of namespace cubic
