#pragma once
#include <vector>

// return numeric_limits<double> on error otherwise spline --> splint
double cubic_interp(const std::vector<double> & xa, // the x values - co ordinate over x axis
                  const std::vector<double> & ya, // the f(x) values -  the values on the y axis
                  double x);                      // x is the location to calcualte "missing" and y is
                                                  // the value at this location
// linear interpulation
std::vector<double> pwl_value_1d(const std::vector<double>& xd,
                                const std::vector<double>& yd,
                                const std::vector<double>& xi);
// nearest interpulation. calcualte the missing values for 
// all entries where we calculating f(xi[i])) over the range of 
// xd and f(xd[i]) and returning the missing values
std::vector<double> nearest_interp_1d (const std::vector<double>& xd,
                                    const std::vector<double> &yd,
                                    std::vector<double> &xi);

// this is the same as pwl_value_1d, only in this case we are
// only calcualting a single missing value of y using f(x)
// and having the x values and f(x) values in x_vals and y_vals
double linear_interpulation_1d(const std::vector<double>& x_vals,
        const std::vector<double>& y_vals, double x_cord);

// same as nearest_interp_1d only calcualting missing value
// for a signle location x_cord of f(x_cord), we have
// the list of x values and their f(x_vals[i]) as well
double nearset_interpolations(const std::vector<double>& x_vals,
        const std::vector<double>& y_vals, double x_cord);

