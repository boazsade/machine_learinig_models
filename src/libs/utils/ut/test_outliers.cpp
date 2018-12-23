#include <boost/test/unit_test.hpp>
#include "outliers_input.h"
#include "utils/math_utils.h"
#include "utils/algorithms.hpp"
#include "inputs/outlier_data.h"
#include "utils/algorithms.hpp"
#include <iostream>

void check_outlier(double expected, std::vector<double>& founds)
{
    auto i = std::find(std::begin(founds), std::end(founds), expected);
    if (i == std::end(founds)) {
        BOOST_TEST(false, "failed to find outliers "<<expected<<" in the list of founds outliers");
    } else {
        founds.erase(i);
    }
}

BOOST_AUTO_TEST_CASE(test_outliers)
{
    // please note that you have a python program to find the outliers for verifiction
    static const double alpha_val = 0.1;
    std::vector<double> samples(std::begin(outliers_data), std::end(outliers_data));
    auto outliers = utils::grubbstest(samples, alpha_val);
    BOOST_TEST(samples.size() != outliers.size());
    BOOST_TEST(samples.size() > outliers.size());
    BOOST_TEST(outliers.size() > 0u);
    // make sure that we dont have any duplications in the outliers
    BOOST_TEST_REQUIRE(utils::size(expected_outliers) == outliers.size()); 
    //std::cout<<"we have "<<outliers.size()<<" outliers that we found in the test out of "<<samples.size()<<std::endl;
    std::sort(std::begin(outliers), std::end(outliers));
    for (auto i = 0u; i < outliers.size(); ++i) {
        BOOST_TEST(outliers[i] == expected_outliers[i]);
        //auto r = verifier.insert(val);
        //BOOST_TEST(r.second, "we are not expecting that the value "<<val<<" would already be inside");
    }
}

BOOST_AUTO_TEST_CASE(test_outliers_with_expected)
{
    static const double alpha_val = 0.1;
    const auto data_size = utils::size(data_outlier);
    const auto number_expected_outliers = utils::size(expected_outlier);
    auto outliers{utils::grubbstest(std::begin(data_outlier), std::end(data_outlier), alpha_val)};
    BOOST_TEST_REQUIRE(number_expected_outliers == outliers.size(), 
            "the number of outliers we have "<<outliers.size()<<" != "<<number_expected_outliers);
    //std::cout<<"we found outliers\n";
    //out_range(std::cout, outliers);
    //std::cout<<"\nexpected outliers:"<<std::endl;
    //for (auto index : expected_outlier) {
    //    if (index < data_size) {
    //        std::cout<<data_outlier[index]<<", ";
    //    } else {
    //        std::cout<<" out of range: "<<index<<", ";
    //    }
    //}
    //std::cout<<std::endl;
    for (auto index : expected_outlier) {   // these are the locations where we expecting to find outliers
        if (index < data_size) {
            auto outlier = data_outlier[index];
            check_outlier(outlier, outliers);
        } else {
            BOOST_TEST_REQUIRE(false, "we have invalid index "<<index<<" out of range for data "<<data_size);
        }
    }
    //std::cout<<std::endl;
}
