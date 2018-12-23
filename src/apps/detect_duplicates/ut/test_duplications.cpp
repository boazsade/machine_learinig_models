#define BOOST_TEST_MODULE TEST_DETECT_OUTLIERS
#include <boost/test/unit_test.hpp>
#include "ut_details/disable_log.h"
#include "utils/algorithms.hpp"
#include "libs/sources_handling/input_data.h"
#include "apps/detect_duplicates/duplication_action.h"
#include "apps/detect_duplicates/column_stats.h"
#include <boost/range/algorithm/generate.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm_ext/push_front.hpp>
#include <boost/range/adaptors.hpp>
#include <iterator>
#include <vector>
#include <iostream>


bool operator == (const input_data& left, const input_data& right)
{
    return left.value == right.value;
}

bool operator < (const input_data& left, const input_data& right)
{
    return left.value < right.value;
}

bool operator != (const input_data& left, const input_data& right)
{
    return !(left == right);
}

#if 0
struct data_generator {
    using result_type = input_data;

    data_generator(unsigned int dups) :
        duplication_entries(dups)
    {
    }

    result_type operator () () const
    {
        if (index % duplication_entries != 0) {
            value += index*1.2;
        }
        return result_type{value, index++};
    }

    mutable result_type::index_type index = 0;
    mutable result_type::value_type value = 0;
    unsigned int duplication_entries = 15;

};


struct make_test_input {
    make_test_input(unsigned int size, unsigned int dup_val) :
        data(size, input_data{})
    {
        boost::generate(data, data_generator{dup_val});
    }

    using data_type = std::vector<input_data>;

    data_type data;
};

BOOST_AUTO_TEST_CASE( test_cleanup_no_action )
{
    static const auto test_data = make_test_input(42, 4);
    // we would like to generate a list without the duplications here
    make_test_input::data_type unique_entries;
    boost::copy(test_data.data | boost::adaptors::uniqued,  std::back_inserter(unique_entries));
    BOOST_TEST_REQUIRE(utils::size(test_data.data) != utils::size(unique_entries));
    std::cout<<"unique entries\n";
    boost::copy(unique_entries, std::ostream_iterator<input_data>(std::cout, ", "));
    std::cout<<"\n-----------------------------------\nall entries\n";
    boost::copy(test_data.data, std::ostream_iterator<input_data>(std::cout, ", "));
    std::cout<<std::endl;
}
#endif

using test_data = std::vector<input_data>;

test_data get_unique_data(unsigned int row_num = 1)
{
    const test_data unique_values {
        {1.12345, ++row_num}, {2.12345, ++row_num}, {3.12345, ++row_num}, {4.12345, ++row_num},
        {5.12365, ++row_num}, {6.12355, ++row_num}, {7.12344, ++row_num}, {8.12333, ++row_num},
        {15.12345, ++row_num}, {16.12345, ++row_num}, {17.12345, ++row_num}, {18.12345, ++row_num},
        {1.12344, ++row_num}, {1.12445, ++row_num}, {1.12, ++row_num}, {1.1241376, ++row_num}
    };
    return unique_values;
}

test_data no_unique_values()
{
    auto uniques = get_unique_data();
    auto unique2 = get_unique_data(uniques.size()+1);
    boost::push_front(unique2, get_unique_data(uniques.size()*2+1));
    boost::push_back(uniques, unique2);
    //std::cout<<"list of none unique values:\n";
    //boost::copy(uniques, std::ostream_iterator<input_data>(std::cout, "\n"));
    //std::cout<<std::endl;
#if 0
    const test_data data {
        uniques[0], {uniques[0].value, 20}, uniques[1], uniques[2],
        {uniques[0].value, 21}, {uniques[0].value, 22}, uniques[3], uniques[4],
        {uniques[2].value, 24}, {uniques[1].value, 25}, uniques[5], uniques[6],
        {uniques[2].value, 26}, {uniques[1].value, 27}, uniques[7], uniques[8],
        {uniques[4].value, 28}, uniques[9], {uniques[9].value, 30}, uniques[10],
        {uniques[1].value, 31}, {uniques[9].value, 32}, {uniques[1].value, 33}, {uniques[4].value, 34},
        {uniques[0].value, 35}, uniques[11], uniques[12], uniques[13], uniques[14],
        uniques[15], {uniques[15].value, 38}, {uniques[14].value, 40}
    };
#endif
    return uniques;
}

BOOST_AUTO_TEST_CASE( test_no_duplicatons )
{
    do_disable_log();

    auto const test_input = get_unique_data();
    duplication_action action{duplication_first{4}};   // the resolution value, we do not really using it here..

    auto total = 0u;
    // process all input
    std::for_each(std::begin(test_input), std::end(test_input), [&total, &action] (auto& entry) {
                ++total;
                ::apply_action(action, entry);
            }
    );
    // check
    BOOST_TEST(total == test_input.size()); // make sure we process all of them
    row_numbers dups, missing;
    finding(action, dups, missing);
    BOOST_TEST(dups.empty(), "there are no duplications in this input!!");
    BOOST_TEST(missing.empty(), "there are no missings in this input!!");

    duplication_action action_last{duplication_last{5}};
    total = 0u;
    std::for_each(std::begin(test_input), std::end(test_input), [&total, &action_last] (auto& entry) {
                ++total;
                ::apply_action(action_last, entry);
            }
    );
    // check
    BOOST_TEST(total == test_input.size()); // make sure we process all of them
    row_numbers dups_last, missing_last;
    finding(action, dups, missing);
    BOOST_TEST(dups_last.empty(), "there are no duplications in this input!!");
    BOOST_TEST(missing_last.empty(), "there are no missings in this input!!");
}

BOOST_AUTO_TEST_CASE( test_duplication_first )
{
    auto test_input = no_unique_values(); // this has duplications
    duplication_action action{duplication_first{6}};   // the resolution value, we do not really using it here..

    auto total = 0u;
    std::for_each(std::begin(test_input), std::end(test_input), [&total, &action] (auto& entry) {
                            ++total;
                            //std::cout<<"processing "<<entry<<"\n";
                            ::apply_action(action, entry);
                 }
    );
    BOOST_TEST(total == test_input.size());
    row_numbers dups, missing;
    finding(action, dups, missing);
    BOOST_TEST(dups.size() > 0, "we are expecting to have duplications here!!");
    BOOST_TEST(missing.empty(), "we should have no missing values");
    //boost::copy(dups, std::ostream_iterator<row_numbers::value_type>(std::cout, "\n"));
    // we are expecting that the duplicated rows would be after the first 1/3
    auto ref_data = get_unique_data();  // these are the rows that we don't expect to find
    for (const auto& entry : ref_data) {
        auto i = std::find_if(dups.begin(), dups.end(), [entry](auto dr) {
                return dr == entry.row;
            }
        );
        BOOST_TEST((i == dups.end()), "the entry "<<entry<<" should not be listed as duplication");
    }
    BOOST_TEST(dups.size() == ref_data.size()*2);
    test_data uniques;  // we would generate a list of unique entries, since we are droping the first
    boost::sort(test_input);
    boost::copy(test_input | boost::adaptors::uniqued,  std::back_inserter(uniques));
    BOOST_TEST_REQUIRE(utils::size(uniques) != utils::size(test_input));
    auto diff = utils::size(test_input) - utils::size(uniques);
    BOOST_TEST_REQUIRE(diff == dups.size());
}

BOOST_AUTO_TEST_CASE( test_duplication_last )
{
    auto test_input = no_unique_values(); // this has duplications
    duplication_action action{duplication_last{6}};   // the resolution value, we do not really using it here..
    auto total = 0u;
    std::for_each(std::begin(test_input), std::end(test_input), [&total, &action] (auto& entry) {
                            ++total;
                            //std::cout<<"processing "<<entry<<"\n";
                            ::apply_action(action, entry);
                 }
    );
    BOOST_TEST(total == test_input.size());
    row_numbers dups, missing;
    finding(action, dups, missing);
    BOOST_TEST(dups.size() > 0, "we are expecting to have duplications here!!");
    BOOST_TEST(missing.empty(), "we should have no missing values");
    //boost::copy(dups, std::ostream_iterator<row_numbers::value_type>(std::cout, "\n"));
    // we are expecting that the duplicated rows would be after the first 1/3
    auto ref_data = get_unique_data(17);  // these are the rows that we don't expect to find
    for (const auto& entry : ref_data) {
        auto i = std::find_if(dups.begin(), dups.end(), [entry](auto dr) {
                return dr == entry.row;
            }
        );
        BOOST_TEST((i == dups.end()), "the entry "<<entry<<" should not be listed as duplication");
    }
    BOOST_TEST(dups.size() == ref_data.size()*2);
}

BOOST_AUTO_TEST_CASE( test_duplication_resolution )
{
    auto test_input = no_unique_values(); // this has duplications
    duplication_action action{duplication_first{2}};   // the resolution value, we do not really using it here..
    auto total = 0u;
    std::for_each(std::begin(test_input), std::end(test_input), [&total, &action] (auto& entry) {
                            ++total;
                            //std::cout<<"processing "<<entry<<"\n";
                            ::apply_action(action, entry);
                 }
    );
    BOOST_TEST(total == test_input.size());
    row_numbers dups, missing;
    finding(action, dups, missing);
    BOOST_TEST(dups.size() > 0, "we are expecting to have duplications here!!");
    BOOST_TEST(missing.empty(), "we should have no missing values");
    //boost::copy(dups, std::ostream_iterator<row_numbers::value_type>(std::cout, "\n"));
    // we are expecting that the duplicated rows would be after the first 1/3
    auto ref_data = get_unique_data();  // these are the rows that we don't expect to find
    for (auto index = 0u; index < (ref_data.size() - 4u); ++index) {
        auto entry = ref_data[index];
        auto i = std::find_if(dups.begin(), dups.end(), [entry](auto dr) {
                return dr == entry.row;
            }
        );
        BOOST_TEST((i == dups.end()), "the entry "<<entry<<" should not be listed as duplication");
    }
    for (auto index = (ref_data.size() - 4u); index < ref_data.size(); ++index) {
        auto entry = ref_data[index];
        auto i = std::find_if(dups.begin(), dups.end(), [entry](auto dr) {
                    return dr == entry.row;
                }
        );
        BOOST_TEST((i != dups.end()), "we are expecting "<<entry<<" to be in duplication list");
    }
    BOOST_TEST(dups.size() == ref_data.size()*2 + 4);
}

