#ifndef DUMP_ROW_MODEL_OUTPUT
#include <boost/test/unit_test.hpp>
#include "libs/sources_handling/row_data_input.h"
#include "libs/model_data/split_rows.h"
#include <iostream>
#include <vector>
#include <string>

struct data4tmodels
{
    using legends_type = std::vector<std::string>;
    using data_list = std::vector<std::vector<mlmodels::value_type>>;

    data4tmodels() : 
        legends{std::string("one"), std::string("two"), std::string("three"), std::string("four")},
        data(make_data(legends.size())),
        data_mat(create(legends, data))
    {

    }

    static data_list make_data(std::size_t size) {
        std::vector<mlmodels::value_type> initial_values{
            10.3f, 43.1f, 90.22f, 0.1f
        };
        data_list data(size);
        for (unsigned int i = 0; i < data.size(); i++) {
            data[i].resize(data4tmodels::size());
        }
        for (unsigned int i = 0; i < data4tmodels::size(); ++i) {
            for (unsigned j = 0; j < data.size(); ++j) {
                data[j][i] = initial_values[j];
                initial_values[j] += 1.f;
            }
        } 
        return data;
    }

    static classifier::read_result create(const legends_type& legends,
            const data_list& data) {
        return classifier::read_result {
            {legends[0], std::begin(data[0]), std::end(data[0])},
            {legends[1], std::begin(data[1]), std::end(data[1])},
            {legends[2], std::begin(data[2]), std::end(data[2])},
            {legends[3], std::begin(data[3]), std::end(data[3])}
        };
    }

    static constexpr unsigned int size()
    {
        return 12u;
    }

    
    legends_type legends;
    data_list data;
    classifier::read_result data_mat;
};

static const data4tmodels models_data = data4tmodels{};

BOOST_AUTO_TEST_CASE(test_empty_data)
{
    column_data cd;
    classifier::read_result rr{cd};
    BOOST_TEST(classifier::empty(rr));
    BOOST_TEST(classifier::columns(rr) == 1u);
    BOOST_TEST(classifier::rows(rr) == 0u);
}

BOOST_AUTO_TEST_CASE(test_row_columns_count)
{
    const column_data::data_type::value_type values[6] = 
        {
            1, 2, 3, 4, 5, 6
    };
    column_data cd{"id", std::begin(values), std::end(values)};
    classifier::read_result rr{cd};
    BOOST_TEST(!classifier::empty(rr));
    BOOST_TEST(classifier::columns(rr) == 1u);
    BOOST_TEST(classifier::rows(rr) == 6u);
}

BOOST_AUTO_TEST_CASE(test_row_columns_counti_multi_columns)
{
    const column_data::data_type::value_type values1[6] = 
        {
            1, 2, 3, 4, 5, 6
    };
    column_data cd1{"id_1", std::begin(values1), std::end(values1)};
    const column_data::data_type::value_type values2[6] = 
        {
            1.1f, 0.1f, 0.01f, 0.001f, 0.0001f, 11.11f
    };
    column_data cd2{"id_2", std::begin(values2), std::end(values2)};
    classifier::read_result rr{cd1, cd2};
    BOOST_TEST(!classifier::empty(rr));
    BOOST_TEST(classifier::columns(rr) == 2u);
    BOOST_TEST(classifier::rows(rr) == 6u);
    try {
        const auto& invalid_col = classifier::data_entry(rr, "no such id");
        (void)invalid_col;
        BOOST_TEST(false, "this should not be, we are expecting an exception");
    } catch (const std::out_of_range&) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting to get out of range error");
    }

    try {
        const auto& d1 = classifier::data_entry(rr, cd1.ID);
        BOOST_TEST_REQUIRE(d1.size() == cd1.data.size());
        const auto& d2 = classifier::data_entry(rr, cd2.ID);
        BOOST_TEST_REQUIRE(d2.size() == cd2.data.size());
        BOOST_TEST_REQUIRE(d1.size() == d2.size());
        for (unsigned int i = 0; i < d1.size(); ++i) {
            BOOST_TEST(values1[i] == d1[i]);
            BOOST_TEST(values2[i] == d2[i]);
        }
    } catch (...) {
        BOOST_TEST(false, "we should not fail here in getting the entries here");
    }
}

BOOST_AUTO_TEST_CASE(test_get_data_train)
{
    const column_data::data_type::value_type values1[6] = 
        {
            1, 2, 3, 4, 5, 6
    };
    column_data cd1{"id_1", std::begin(values1), std::end(values1)};
    const column_data::data_type::value_type values2[6] = 
        {
            1.1f, 0.1f, 0.01f, 0.001f, 0.0001f, 11.11f
    };
    column_data cd2{"id_2", std::begin(values2), std::end(values2)};
    const column_data::data_type::value_type values3[6] = 
        {
            100, 2.1f, 13.3, 1.4f, 59, 0.6f
    };
    column_data labels{"label column", std::begin(values3), std::end(values3)};
    const column_data::data_type::value_type values4[6] = 
        {
            11.1f, 0.55f, 0.078f, 0.101f, 0.5001f, 11.99f
    };
    column_data cd4{"id_4", std::begin(values4), std::end(values4)};
    classifier::read_result rr{cd1, cd2, labels, cd4};
    BOOST_TEST_REQUIRE(!classifier::empty(rr));
    BOOST_TEST_REQUIRE(classifier::columns(rr) == 4u);
    BOOST_TEST_REQUIRE(classifier::rows(rr) == 6u);

    auto output = classifier::data_matrix(rr, labels.ID, 20u);  // number of rows that we pass is too large, lets see how this works
    BOOST_TEST_REQUIRE(output.size() == cd1.data.size());            // we have 3 columns
    BOOST_TEST_REQUIRE(output[0].size() == 3u);    // number of columns same as input data
    for (unsigned int j = 0; j < output.size(); ++j) {
        BOOST_TEST(values1[j] == output[j][0]);
        BOOST_TEST(values2[j] == output[j][1]);
        BOOST_TEST(values4[j] == output[j][2]);
    }
#if 0
    for (unsigned int i = 0; i < output.size(); ++i) {
        for (unsigned int j = 0; j < output[i].size(); ++j) {
            std::cout<<output[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
#endif
}

BOOST_AUTO_TEST_CASE(test_calcuate_precents)
{
    auto s = classifier::spliter_at(20u, 100u); // this should return 80
    BOOST_TEST(s == (100u - 20u));
    s = classifier::spliter_negate(20u, 100u);  // and this should be 20
    BOOST_TEST((unsigned)s == (100u - 80u));
}

BOOST_AUTO_TEST_CASE(test_exterct_data)
{
    BOOST_TEST_REQUIRE(classifier::rows(models_data.data_mat) == data4tmodels::size());
    BOOST_TEST_REQUIRE(classifier::columns(models_data.data_mat) == models_data.legends.size());
    // now lets see if we can get the "correct" data
    auto mat = classifier::data_matrix(models_data.data_mat, models_data.legends[1], 1u, 1u);   // only one row start from the second row
    BOOST_TEST_REQUIRE(mlmodels::rows(mat) == 1u);                    // verify one row
    BOOST_TEST_REQUIRE(mlmodels::columns(mat) == models_data.legends.size()-1u);     // and that we have one less column
    // check what we have in the result matrix..
    // note that since we don't have data from second array, we skip it here
    BOOST_TEST(mat[0][0] == models_data.data[0][1]);
    BOOST_TEST(mat[0][1] == models_data.data[2][1]);
    BOOST_TEST(mat[0][2] == models_data.data[3][1]);
}

BOOST_AUTO_TEST_CASE(test_class_data)
{
    auto classes = classifier::transform2labels(models_data.data_mat, models_data.legends[2], 2u, 3u);  // take 2 values starting from 3rd entry
    auto labels = classifier::labels_data(models_data.data_mat, models_data.legends[2], 2u, 3u);        // take 2 values starting from 3rd entry
    BOOST_TEST_REQUIRE(classes.size() == 2u);
    BOOST_TEST_REQUIRE(labels.size() == 2u);
    unsigned int j = 3u;    // the location with the initial data
    for (unsigned int i =0; i < classes.size(); ++i, ++j) {
        BOOST_TEST(labels[i] == models_data.data[2][j]);
        BOOST_TEST(classes[i] != models_data.data[2][j]);
    }
    for (int i =0; i < (int)classes.size(); ++i) {
        BOOST_TEST(classes[i] == i+1);
    }

}

BOOST_AUTO_TEST_CASE(test_class_data_offset)
{
    // check what would happen if we are passing out of range values
    auto const size = data4tmodels::size();
    auto labels = classifier::labels_data(models_data.data_mat, models_data.legends[3], 
                                          size+7u, 0u);        // take more than what we have
    BOOST_TEST(labels.size() == size);
    for (unsigned int i =0; i < labels.size(); ++i) {
        BOOST_TEST(labels[i] == models_data.data[3][i]);
    }

    labels = classifier::labels_data(models_data.data_mat, models_data.legends[1],
                                    4u, size - 2u);           // the offset takes us out of range!
    BOOST_TEST(labels.size() == 2u);
    unsigned int j = size - 2u;
    for (unsigned int i =0; i < labels.size(); ++i, ++j) {
        BOOST_TEST(labels[i] == models_data.data[1][j]);
    }
}

BOOST_AUTO_TEST_CASE(test_data_with_out_of_range)
{
    auto samples = classifier::data_matrix(models_data.data_mat, models_data.legends[0],
                                data4tmodels::size(), 2u);      // the offset would take it out of range
    BOOST_TEST(mlmodels::rows(samples) == (data4tmodels::size() - 2u));
    unsigned int j = 2u;
    for (unsigned int i = 0; i < mlmodels::rows(samples); i++, j++) {
        BOOST_TEST(samples[i][0] == models_data.data[1][j]);
        BOOST_TEST(samples[i][1] == models_data.data[2][j]);
        BOOST_TEST(samples[i][2] == models_data.data[3][j]);
    }

}
#endif  // DUMP_ROW_MODEL_OUTPUT
