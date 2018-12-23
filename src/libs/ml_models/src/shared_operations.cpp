#include "shared_operations.hpp"
#include <algorithm>
//#include <map>

namespace mlmodels { namespace detail {


class_data::value_type predict_prob(class_data::value_type from)
{
    return 1/(1 + std::exp(-from));
}

inline
cv::Mat convert2(const classes_data& from, int type)
{
    cv::Mat m(from.size(), 1, type, 0.0f);
    int row = 0;
    for (auto v : from) {
        m.at<float>(row, 0) = static_cast<float>(v);
        ++row;
    }
    return m;
}

cv::Mat convert3(const class_data& from, int type)
{

    cv::Mat m(from.size(), 1, type, 0.0f);
    int row = 0;
    for (auto v : from) {
        m.at<int>(row++, 0) = static_cast<float>(v);
    }
    return m;
#if 0
//    std::map<class_data::value_type, int> mapper;

    cv::Mat m(from.size(), 1, type, 0.0f);

    int row = 0;
    int val = 1;
    for (auto v : from) {
//        auto r = mapper.insert(std::make_pair(v, val));
//        auto vv = r.first->second;
//        if (r.second) {
//            vv = val;
//            ++val;
        }
        m.at<int>(row, 0) = static_cast<float>(v);
        ++row;
    }
    if (m.rows == (int)mapper.size() ||
        mapper.size() < 2) {
        throw std::runtime_error{"invalid labels passed - either not real labeled data or not enouth classes"};
    }
    return m;
#endif
}

void copy_result(const cv::Mat& from, testing_result& to,
        std::size_t expected_size)
{
    static_assert(std::is_same<testing_result::value_type, float>::value,
            "we are expecting to have floating type here!!");
    std::size_t ms = from.rows * from.cols;
    if (ms == expected_size) {
        for (auto i = 0; i < from.rows; ++i) {
            for (auto j = 0; j < from.cols; ++j) {
                to.push_back(from.at<testing_result::value_type>(i, j));
            }
        }
    }
}


class_data  copy_result(const cv::Mat& from, std::size_t expected_size)
{
    std::size_t ms = from.rows * from.cols;
    class_data output;
    if (ms == expected_size) {
        output.reserve(ms);
        for (auto i = 0; i < from.rows; ++i) {
            for (auto j = 0; j < from.cols; ++j) {
                output.push_back(from.at<testing_result::value_type>(i, j));
            }
        }
    }
    return output;
}

cv::Mat prepare_samples(const class_data& from, int type)
{
    cv::Mat m(1, from.size(), type, (void*)from.data());
    return m;
}

cv::Mat prepare_samples(const training_data& from, int type)
{
    cv::Mat m(rows(from), columns(from),  type, (void*)from.data());
    if (m.cols != (int)columns(from) || m.rows != (int)rows(from)) {
        throw std::runtime_error{"error while prepering samples - internal error"};
    }
    return m;
}

cv::Mat prepare_labels(const classes_data& from, int type)
{
    switch (type) {
        case CV_32S:
        case CV_32SC2:
        case CV_32SC3:
        case CV_32SC4:
            return cv::Mat(from.size(), 1,  type, (void*)from.data());
        case CV_32F:
        case CV_32FC2:
        case CV_32FC3:
        case CV_32FC4:
            return convert2(from, type);
        default:
            throw std::runtime_error{
                "we cannot support this type " + std::to_string(type)
            };
    }
}

cv::Mat prepare_labels(const class_data& from, int type)
{
    switch (type) {
        case CV_32S:
        case CV_32SC2:
        case CV_32SC3:
        case CV_32SC4:
            return convert3(from, type);
        case CV_32F:
        case CV_32FC2:
        case CV_32FC3:
        case CV_32FC4:
            return cv::Mat(from.size(), 1,  type, (void*)from.data());
        default:
            throw std::runtime_error{
                "we cannot support this type " + std::to_string(type)
            };
    }


}

}   // end of detail 
}   // end of classifier
