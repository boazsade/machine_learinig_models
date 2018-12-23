#pragma once
#include <iosfwd>
#include <type_traits>


namespace mlmodels
{

struct classifier_model
{
};
std::ostream& operator << (std::ostream& os, classifier_model);

struct regression_model
{
};

std::ostream& operator << (std::ostream& os, regression_model);

inline 
constexpr bool is_classifier(classifier_model) {
    return true;
}

inline
constexpr bool is_regression(classifier_model) {
    return false;
}

inline 
constexpr bool is_classifier(regression_model) {
    return false;
}

inline
constexpr bool is_regression(regression_model) {
    return true;
}

template<typename T>
inline constexpr bool is_classifier()
{
    return std::is_base_of<classifier_model, T>::value;;
}

template<typename T>
inline constexpr bool is_regression()
{
    return std::is_base_of<regression_model, T>::value;
}

}   // end of namespace mlmodels


