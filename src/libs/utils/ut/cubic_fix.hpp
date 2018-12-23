#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#pragma once

std::vector<double>  spline (const std::vector<double> &x, const std::vector<double> &y)
 {
     
        double qn, un;
        int n=(int)x.size();
        std::vector<double>  y2(x.size(), 0);
        //  Allocate memory for the vector.
        std::vector<double> u(x.size(), 0);
        
        //  Compute first derivatives at point 1 and point n.
        double yp1 = ( y[1] - y[0] ) / ( x[1] - x[0] );
        double ypn = ( y[n-1] - y[n-2] ) / ( x[n-1] - x[n-2] );
        
        //  Spline computation.
        if ( yp1 > std::numeric_limits<double>::max() )
            y2[0] = u[0] = 0.0;
        else
        {
            y2[0] = -0.5;
            u[0] = ( 3.0 / ( x[1] - x[0] ) ) * ( ( y[1] - y[0] ) / ( x[1] - x[0] ) - yp1 );
        }
        
        for ( int i = 1;i <= n - 2; ++i )
        {
            double sig = ( x[i] - x[i-1] ) / ( x[i+1] - x[i-1] );
            double p = sig * y2[i-1] + 2.0;
            y2[i] = ( sig - 1.0 ) / p;
            u[i] = ( y[i+1] - y[i] ) / ( x[i+1] - x[i] ) - ( y[i] - y[i-1] ) / ( x[i] - x[i-1] );
            u[i] = ( 6.0 * u[i] / ( x[i+1] - x[i-1] ) - sig * u[i-1] ) / p;
        }
        
        if ( ypn >  std::numeric_limits<double>::max() )
            qn = un = 0.0;
        else
        {
            qn = 0.5;
            un = ( 3.0 / ( x[n-1] - x[n-2] ) ) * ( ypn - ( y[n-1] - y[n-2] ) / ( x[n-1] - x[n-2] ) );
        }
        
        y2[n-1] = ( un - qn * u[n-2] ) / ( qn * y2[n-2] + 1.0 );
        
        for ( int k = n - 2;k >= 0;k-- )
            y2[k] = y2[k] * y2[k+1] + u[k];
     
     return y2;
    }
    
    //*****************************************************************************
    //  function: int splint(double *xa, double *ya, double *y2, int n,
    //                          double x, double *y)
    //  Compute the second derivative values for the interpolating function.
    //  Input:  *xa     -   tabulated x values
    //      *ya     -   tabulated y=f(x) values
    //      *y2     -   output from spline()
    //      n       -   size of the arrays
    //      x       -   input x value
    //  Output: *y      -   interpolated y value
    //  Returns:        0 = success
    //              1 = error
    //
    //  Slightly modified code from Numerical Recipes in C, Section 3.3
    //*****************************************************************************
    double  splint (const std::vector<double> & xa,
                const std::vector<double> & ya,
                const std::vector<double> & y2,
                double x)
    {
        int klo, khi, k;
        double h, b, a;
        int  const n=(int)xa.size();
        klo = 0;
        khi = n;
        //binary search
        while ( khi - klo > 1 )
        {
            k = ( khi + klo ) >> 1;
            if ( xa[k] > x )
                khi = k;
            else
                klo = k;
        }
        
        h = xa[khi] - xa[klo];
        if (h == 0.0) {
            std::numeric_limits<double>::max();
        }
        a = ( xa[khi] - x ) / h;
        b = ( x - xa[klo] ) / h;
        return a * ya[klo] + b * ya[khi] + ( ( std::pow ( a, 3 ) - a ) * y2[klo] + ( std::pow ( b, 3 ) - b ) * y2[khi] ) * ( h * h ) / 6.0;
        
    }
std::vector<double>  interpolate(const std::vector<double> & xa,
                  const std::vector<double> & ya,
                  const std::vector<double> & x) 
{
    auto  y = spline(xa, ya);
    std::vector<double> output(x.size(), 0);
    std::transform(std::begin(x), std::end(x), std::begin(output), [&xa, &ya, &y](auto i) {
                return splint(xa, ya, y, i); 
        }
    );
    return output;
}

