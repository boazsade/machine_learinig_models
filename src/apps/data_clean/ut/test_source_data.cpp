#include <boost/test/unit_test.hpp>
#include "shared_operations.h"
#include "libs/sources_handling/source_data.h"
#include "libs/sources_handling/input_data.h"
#include "ut_details/disable_log.h"
#include <boost/iterator/function_input_iterator.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

std::ostream& operator << (std::ostream& os, const std::vector<double>& data);
std::ostream& operator << (std::ostream& os, std::vector<double>::iterator )
{
    return os<<"double iterator";
}

std::ostream& operator << (std::ostream& os, std::vector<double>::const_iterator )
{
    return os<<"double iterator";
}

struct generate_seq
{
    using result_type = input_data;

    generate_seq(double iv, double s) :
        value(iv), step(s)
    {
    }

    result_type operator () () const
    {
        auto t = value;
        value = value + step;
        return result_type{t, index++};
    }
    
    mutable double value = 0.0;
    double step = 0.0;
    mutable unsigned int index = 0;
};

struct generate_nums
{
    using result_type = double;

    explicit generate_nums(const generate_seq& gs) :
        generator(gs)
    {
    }

    result_type operator () () const
    {
        return generator().value;
    }

    generate_seq generator;
};

source_data test_source_data(generate_seq&& gs, const std::string& id, unsigned int count)
{
    return source_data{id, boost::make_function_input_iterator(gs, 0u),
                        boost::make_function_input_iterator(gs, count)};
}

std::ostream& operator << (std::ostream& os, const std::vector<double>& data)
{
    std::copy(std::begin(data), std::end(data), std::ostream_iterator<double>(os, " "));
    return os;
}

BOOST_AUTO_TEST_CASE(test_split_out)
{
    do_disable_log();
    auto test_input = make_test_data(20, "some id");
    auto data_slice = slice_it(test_input, 40, 4); // should fail, no such index
    BOOST_TEST(data_slice.first == data_slice.second, "should return empty range first == second");
    BOOST_TEST(data_slice.first == test_input.end(), "should return empty range equal to test set end");

    data_slice = slice_it(test_input, 1, 3);
    BOOST_TEST(data_slice.first != data_slice.second, "should return valid range first != second");
    BOOST_TEST(data_slice.first != test_input.end(), "should return valid range not equal to test set end");
    BOOST_TEST(std::distance(data_slice.first, data_slice.second) == 4, 
            "we are taking the first entry as pivot so size is "<<std::distance(data_slice.first, data_slice.second));
    data_slice = slice_it(test_input, 3, 2);
    BOOST_TEST(data_slice.first != data_slice.second, "should return valid range first != second");
    BOOST_TEST(data_slice.first != test_input.end(), "should return valid range not equal to test set end");
    BOOST_TEST(std::distance(data_slice.first, data_slice.second) == 5,
            "valid input for this as this is in the middle of the range: "<<std::distance(data_slice.first, data_slice.second));
    data_slice = slice_it(test_input, 23, 4);
    BOOST_TEST(data_slice.first != data_slice.second, "should return valid range first != second");
    BOOST_TEST(data_slice.first != test_input.end(), "should return valid range not equal to test set end");
    BOOST_TEST(data_slice.second == test_input.end(), "should return valid range not equal to test set end");
    BOOST_TEST(std::distance(data_slice.first, data_slice.second) == 5,
            "valid input for this as this is in the end of the range: "<<std::distance(data_slice.first, data_slice.second));
    //std::cout<<"input data:\n"<<test_input<<std::endl;
}

BOOST_AUTO_TEST_CASE(test_transform_to_algorithm)
{
    constexpr auto initial_value = 1.0;
    constexpr auto step_size = 3.2;
    constexpr unsigned int expected_sub_size = 5u;

    auto data_source = test_source_data(generate_seq{initial_value, step_size}, "test_id", 20);
    auto data_slice = slice_it(data_source, 4, 2);
    BOOST_TEST_REQUIRE(data_slice.first != data_slice.second, "should return valid range first != second");
    BOOST_TEST_REQUIRE(std::distance(data_slice.first, data_slice.second) == expected_sub_size, 
            "we must have a valid range of "<<std::distance(data_slice.first, data_slice.second)<<" elements");
    BOOST_TEST_REQUIRE(data_slice.first->row == 2, "we are expecting that the index for the first entry is "<<data_slice.first->row);
    BOOST_TEST_REQUIRE(data_slice.second->row == 7, "we are expecting that the index for the second entry is "<<data_slice.second->row);

    generate_nums gn{generate_seq {data_slice.first->value, step_size}};
    source_data::algo_input controled_data{boost::make_function_input_iterator(gn, 0u), 
                                           boost::make_function_input_iterator(gn, expected_sub_size)
                                    };
    auto invalid_entry = data_slice.first + 2;
    auto generated_seq = transform_slice(data_slice, invalid_entry->value);
#if 0
    std::copy(std::begin(controled_data), std::end(controled_data), std::ostream_iterator<double>(std::cout, " "));
    std::cout<<std::endl;
    std::copy(std::begin(generated_seq), std::end(generated_seq), std::ostream_iterator<double>(std::cout, " "));
    std::cout<<std::endl;
    std::copy(data_slice.first, data_slice.second, std::ostream_iterator<input_data>(std::cout, " "));
    std::cout<<std::endl;
#endif
    BOOST_TEST(generated_seq.size() + 1 == controled_data.size(), "we expecting to get the same size "<<generated_seq.size()<<" for both sequences");
    auto it = (std::find(std::begin(generated_seq), std::end(generated_seq), invalid_entry->value) == std::end(generated_seq));
    BOOST_TEST_REQUIRE(it , "we don't expect to find "<<invalid_entry->value<<" in the squence");
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data), std::end(controled_data)) == false, 
            "we expecting both range to not have the same data, we've got as  we removed "<<invalid_entry->value);
    generated_seq.push_back(invalid_entry->value);
    std::sort(std::begin(generated_seq), std::end(generated_seq));
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data), std::end(controled_data)), 
            "we expecting both range to have the same data, we've got as we insert back "<<invalid_entry->value);
}

BOOST_AUTO_TEST_CASE(test_bound_get_subarray)
{
    constexpr auto initial_value = 2.1;
    constexpr auto step_size = 4.3;
    constexpr unsigned int dataset_size = 17;
    constexpr unsigned int subrange_size = 5;

    auto data_source = test_source_data(generate_seq{initial_value, step_size}, "test_id", dataset_size);
    auto data_slice = slice_it(data_source, 0, subrange_size); // we are taking "not enough space" at the start
    BOOST_TEST_REQUIRE(data_slice.first != data_slice.second, "this must not be an empty dataset "<<std::distance(data_slice.first, data_slice.second));
    BOOST_TEST_REQUIRE(data_slice.first == data_source.begin(), "the first element is expected to be the first in the input range"<<*data_slice.first);
    BOOST_TEST_REQUIRE(data_slice.first->row == 0, "the first element must have row number 0 "<<data_slice.first->row);
    BOOST_TEST_REQUIRE(std::distance(data_slice.first, data_slice.second) == subrange_size+1, 
            "the number of elements in the sub range is expected to be "<<std::distance(data_slice.first, data_slice.second));
    generate_nums gn{generate_seq {data_slice.first->value, step_size}};
    source_data::algo_input controled_data{boost::make_function_input_iterator(gn, 0u),
                                        boost::make_function_input_iterator(gn, subrange_size+1)
                        };
    auto invalid_entry = data_slice.first;
    auto generated_seq = transform_slice(data_slice, invalid_entry->value);
    BOOST_TEST(generated_seq.size() + 1 == controled_data.size(), "we expecting to get the same size "<<generated_seq.size()<<" for both sequences");
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data), std::end(controled_data)) == false, 
            "we expecting both range to have different data, as one is missing "<<invalid_entry->value);
    BOOST_TEST((std::find(std::begin(generated_seq), std::end(generated_seq), invalid_entry->value) == std::end(generated_seq)), 
            "we don't want to have the value "<<invalid_entry->value<<" in our sub range");
    generated_seq.push_back(invalid_entry->value);
    std::sort(std::begin(generated_seq), std::end(generated_seq));
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data), std::end(controled_data)) , 
            "we expecting both range to have the same data, we've got");

    data_slice = slice_it(data_source, data_source.row_data()[size(data_source)-1].row, subrange_size);
    BOOST_TEST_REQUIRE(data_slice.first != data_slice.second, "this must not be an empty dataset "<<std::distance(data_slice.first, data_slice.second));
    BOOST_TEST_REQUIRE(data_slice.second == data_source.end(), "the last element is expected to be the last in the input range"<<*data_slice.first);
    BOOST_TEST_REQUIRE(std::distance(data_slice.first, data_slice.second) == subrange_size+1, "the number of elements in the sub range is expected to be "<<
                std::distance(data_slice.first, data_slice.second));
    generate_nums gn2{generate_seq {data_slice.first->value, step_size}};
    source_data::algo_input controled_data2{boost::make_function_input_iterator(gn2, 0u),
                                        boost::make_function_input_iterator(gn2, subrange_size+1)
                        };
    generated_seq = transform_slice(data_slice,  data_source.row_data()[size(data_source)-1].value);
    BOOST_TEST(generated_seq.size() + 1 == controled_data2.size(), "we expecting to get the same size "<<generated_seq.size()<<" for both sequences");
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data2), std::end(controled_data2)) == false, 
            "we expecting both range to not be the same , as we removed "<< data_source.row_data()[size(data_source)-1]);
    generated_seq.push_back(data_source.row_data()[size(data_source)-1].value);
    std::sort(std::begin(generated_seq), std::end(generated_seq));
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data2), std::end(controled_data2)) ,
                        "we expecting both range to be the same , as we added back "<< data_source.row_data()[size(data_source)-1]);

}

BOOST_AUTO_TEST_CASE(test_merge_from_slice)
{
    constexpr auto initial_value = 5.81;
    constexpr auto step_size = 6.9;
    constexpr unsigned int dataset_size = 543;
    constexpr unsigned int subrange_size = 4;
    constexpr unsigned int row_number = dataset_size/2;
    static const double new_entry_data = 5.12345;

    auto data_source = test_source_data(generate_seq{initial_value, step_size}, "test_id", dataset_size);
    auto data_slice = slice_it(data_source, row_number, subrange_size); // we are taking "not enough space" at the start
    BOOST_TEST_REQUIRE(data_slice.first != data_slice.second, "this must not be an empty dataset "<<std::distance(data_slice.first, data_slice.second));
    BOOST_TEST_REQUIRE(std::distance(data_slice.first, data_slice.second) == subrange_size*2+1, 
           "we are expecting to have dataset with size "<<(subrange_size*2+1)<<" and not "<<std::distance(data_slice.first, data_slice.second));
    BOOST_TEST_REQUIRE(data_slice.second != data_source.end(), "the second element in the list must not be the last");
    generate_nums gn{generate_seq {data_slice.first->value, step_size}};
    source_data::algo_input controled_data{boost::make_function_input_iterator(gn, 0u),
                                        boost::make_function_input_iterator(gn, subrange_size*2+1)
                        };
    auto generated_seq = transform_slice(data_slice);
    BOOST_TEST(generated_seq.size() == controled_data.size(), "we expecting to get the same size "<<generated_seq.size()<<" for both sequences");
    BOOST_TEST(std::equal(std::begin(generated_seq), std::end(generated_seq), std::begin(controled_data), std::end(controled_data)), 
            "we expecting both range to have the same data, we've got");
    // now we would like to manipulate the data we have and merge it back at the same location
    source_data::algo_input changed_data;
    std::transform(std::begin(generated_seq), std::end(generated_seq), std::back_inserter(changed_data), [](double) {
                                return new_entry_data;
                            }
                );
    BOOST_TEST_REQUIRE(changed_data.size() == generated_seq.size());
    BOOST_TEST_REQUIRE(changed_data.size() == subrange_size*2+1);
    BOOST_TEST_REQUIRE(std::equal(std::begin(changed_data), std::end(changed_data),
                                 std::begin(generated_seq), std::end(generated_seq)) == false, "must have different data set for next step");
    auto size_before_merge = size(data_source);
    auto data_before_merge = data_source;
    merge_slice(changed_data, data_slice);
    BOOST_TEST_REQUIRE(size(data_source) == size_before_merge);
    BOOST_TEST_REQUIRE(size(data_source) == dataset_size);
    BOOST_TEST_REQUIRE(std::equal(std::begin(data_source), std::end(data_source),
                                   std::begin(data_before_merge), std::end(data_before_merge)) == false,
                        "the data after the merge must not be the same in content as the data before the merge");
    auto i = std::find_if(data_slice.first, data_slice.second, [](auto entry) {
                                    return entry.value == new_entry_data;
                                } 
                        );
    BOOST_TEST_REQUIRE(i == data_slice.second, "we are exepecting to have the sub range with all values == "<<new_entry_data);
}
