#include <boost/test/unit_test.hpp>
#include "inputs/interp_tests.h"
#include "slice4missing.hpp"
#include "utils/algorithms.hpp"
#include "utils/cubic_iterpolation.h"
//#include "utils/math_utils.h"
#include "cubic_fix.hpp"
void remove_ambiguity_from_data(std::vector<double> &yd, 
        std::vector<double> &xd,
        const std::vector<double> &xi)
{
    for (size_t y=0;y<xi.size();y++) {
        auto it = std::find(xd.begin(), xd.end(), xi[y]);

        if(it!=xd.end()){
            auto index = std::distance(xd.begin(), it);
            xd.erase(xd.begin()+index);
            yd.erase(yd.begin()+index);

        }

    }

}


BOOST_AUTO_TEST_CASE(test_cubic)
{
    /*
    auto dataset = from_double_array(std::begin(values), std::end(values), "rand_name");
    auto missing_begin = std::begin(xi_interp);
    auto missing_end = std::end(xi_interp);
    BOOST_TEST_REQUIRE(utils::size(expected_nearest_interp) == utils::size(xi_interp));  // make sure input is valid!!
    std::size_t count = 0u;
    while (missing_begin != missing_end) {
        auto expected = expected_cubic_interp[count];
        auto subset = slice(dataset, missing_begin, 2u);    // the the subset that we need to fix
        BOOST_TEST_REQUIRE(utils::size(subset) == 4u);        // win size * 2
        const auto vals = transform_slice(std::begin(subset), std::end(subset));
        const auto cords = row_nums_transform(std::begin(subset), std::end(subset));
        BOOST_TEST_REQUIRE(vals.size() == cords.size());
        BOOST_TEST_REQUIRE(vals.size() == subset.size());
        auto calc = utils::cubic_interpolation(vals, cords,
                static_cast<utils::fx_list_type::value_type>(*missing_begin));
        if (!round_cmp(expected, calc, 0.0001)) { 
            std::cout<<"input is [";
            out_range(std::cout, subset, " ");
            std::cout<<"] at location [";
            out_range(std::cout, cords, " ");
            std::cout<<"] ";
            BOOST_TEST(false, "- fix value "<<calc<<" != expected "<<expected<<" index "<<*missing_begin<<" for value "<<values[*missing_begin]);
        }
        ++missing_begin;
        ++count;
    }
    */
    std::vector<double> data_set(std::begin(values), std::end(values));
    std::vector<double> coordinates;
    std::transform(std::begin(xd_interp), std::end(xd_interp), std::back_inserter(coordinates), [](auto val) {
                            return static_cast<double>(val);
                        }
    );
    std::vector<double> missing_values;
    std::transform(std::begin(xi_interp), std::end(xi_interp), std::back_inserter(missing_values), [](auto val) {
                            return static_cast<double>(val);
                        }
    );
    remove_ambiguity_from_data(data_set, coordinates, missing_values);
    BOOST_TEST_REQUIRE(utils::size(missing_values) == utils::size(expected_cubic_interp));
    auto corrections = interpolate(coordinates, data_set, missing_values);
    BOOST_TEST_REQUIRE(corrections.size() == utils::size(expected_cubic_interp));
    auto start = std::begin(expected_cubic_interp);
    for (auto i = std::begin(corrections); i != std::end(corrections); ++i, ++start) {
        BOOST_TEST_REQUIRE(round_cmp(*i, *start, 0.0001), "the calcualted value "<<*i<<" != "<<*start);
    }
}

BOOST_AUTO_TEST_CASE(test_cubic2)
{
    std::vector<double> data_set(std::begin(values), std::end(values));
    std::vector<double> coordinates;
    std::transform(std::begin(xd_interp), std::end(xd_interp), std::back_inserter(coordinates), [](auto val) {
                            return static_cast<double>(val);
                        }
    );
    std::vector<double> missing_values;
    std::transform(std::begin(xi_interp), std::end(xi_interp), std::back_inserter(missing_values), [](auto val) {
                            return static_cast<double>(val);
                        }
    );
    remove_ambiguity_from_data(data_set, coordinates, missing_values);
    BOOST_TEST_REQUIRE(utils::size(missing_values) == utils::size(expected_cubic_interp));
    const auto slist = spline(coordinates, data_set);
    BOOST_TEST_REQUIRE(slist.size() == data_set.size());
    
    auto start = std::begin(expected_cubic_interp);
    for (auto i = std::begin(missing_values); i != std::end(missing_values); ++i, ++start) {
        auto correction = splint(coordinates, data_set, slist, *i);
        BOOST_TEST_REQUIRE(round_cmp(correction, *start, 0.0001), "calcuated value "<<correction<<" != expected value "<<*start);
    }
}

BOOST_AUTO_TEST_CASE(test_cubic_code)
{
    auto dataset = from_double_array(std::begin(values), std::end(values), "rand_name");
    auto size_before = size(dataset);
    auto new_end = filter_out(begin(dataset), end(dataset), std::begin(xi_interp), std::end(xi_interp));
    auto size_after_filter = std::distance(begin(dataset), new_end);
    BOOST_TEST_REQUIRE(size_before > size_after_filter);
    try {
        cubic::spline_data inrep_data{row_nums_transform(begin(dataset), new_end),
                    transform_slice(begin(dataset), new_end)
                };
        for (auto i = 0u; i < utils::size(xi_interp); ++i) {
            auto expected = expected_cubic_interp[i];
            auto before = values[xi_interp[i]];
            BOOST_TEST(expected != before);
            auto fixed = cubic::interpole(inrep_data, xi_interp[i]);
            BOOST_TEST(fixed != before);
            BOOST_TEST(round_cmp(fixed, expected, 0.0001), "the fixed value "<<fixed<<" != "<<expected<<" for value at "<<xi_interp[i]);
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "failed to run test: "<<e.what());
    }
                           
}
