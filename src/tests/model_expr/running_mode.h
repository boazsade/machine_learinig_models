#pragma once
#include <iostream>
#include <string>

// This is the logic by which the train/test data is split with. 
// We can have two modes - 
// 1. split the data with a given precent value so that we would test and train data
// 2. k-fold mode that split the data into given number of folds
// the classes here are only the "configuration" that is being use and as
// a "tag" that we would be overloaded upon so that when an action is apply
// on, it would be use tranperently to split the data for that use this logic
namespace classifier
{

struct percent_split
{
    static const unsigned int max_value = 100u;
    percent_split() = default;

    percent_split(unsigned int s,
            const std::string& tid);

    unsigned int split = max_value;   // can be between 100 and 1
    std::string  target_id;           // this is the name of the column that 
                                      // is not used for training/testing
};
std::ostream& operator << (std::ostream& os, const percent_split& ps);


struct k_fold
{
    k_fold() = default;
    k_fold(unsigned int s, 
        const std::string& tid);

    unsigned int split = 1;
    std::string  target_id;           // this is the name of the column that 
                                      // is not used for training/testing
};  
std::ostream& operator << (std::ostream& os, const k_fold& kf);

template<typename Logic>
struct test_run;

template<>
struct test_run<percent_split> : percent_split
{
    using percent_split::percent_split;
};

template<>
struct test_run<k_fold> : k_fold
{
    using k_fold::k_fold;
};

template<typename Logic>
struct train_run;
template<>
struct train_run<percent_split> : percent_split
{
    using percent_split::percent_split;
};

template<>
struct train_run<k_fold> : k_fold
{
    using k_fold::k_fold;
};

template<typename Logic> inline
std::ostream& operator << (std::ostream& os, const test_run<Logic>& t)
{
    return os<<static_cast<const Logic&>(t);
}

template<typename Logic> inline
std::ostream& operator << (std::ostream& os, const train_run<Logic>& t)
{
    return os<<static_cast<const Logic&>(t);
}

}   // end of namespace classifier
