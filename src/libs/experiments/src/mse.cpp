#include "libs/experiments/mse.h"
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/size.hpp>
#include <boost/range/numeric.hpp>
#include <functional>
//#define PRINT_DATA_FOR_ALGO
#ifdef PRINT_DATA_FOR_ALGO
#   include <iostream>
#   include <iterator>
#   include <algorithm>
#   define PRINTOUT_DATA(_data)  std::copy(std::begin(_data), std::end(_data), std::ostream_iterator<value_type>(std::cout, ", ")); std::cout<<std::endl
#else
#    define PRINTOUT_DATA(_data) 
#endif  // PRINT_DATA_FOR_ALGO

/*

#include <opencv/cxcore.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

namespace exprs
{
float mse(cv::Mat &a,cv::Mat &b)
{
    auto N = a.rows;
    auto c = a - b;
    c=c.t()*c;
    return c.at<float>(0,0)/N;
}
}	// end of namespace exprs
*/
namespace exprs
{ 

mse_results mse(const std::vector<value_type>& predictions,
	const std::vector<value_type>& mesured)
{
    if (boost::size(predictions) != boost::size(mesured)) {
        throw std::runtime_error{"both ranges must be same prediction = " + 
		std::to_string(boost::size(predictions)) + ", mesured = " +
		std::to_string(boost::size(mesured))};
    }
    PRINTOUT_DATA(predictions);
    PRINTOUT_DATA(mesured);
    std::vector<value_type> tmp_res(boost::size(mesured), 0.f);
    boost::transform(predictions, mesured, std::begin(tmp_res), std::minus<value_type>()); 
    return mse_results{boost::inner_product(tmp_res, tmp_res, 0.f)}; 
}

}	// end namespace exprs

