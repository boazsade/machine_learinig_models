#pragma once
#include "libs/ml_models/general_types.h"
#include <boost/range/algorithm/generate.hpp>
#include <cstdlib>

namespace ut_data
{

const int NUMBER_OF_CLASSES = 6;

static const float row_data[][3] = {
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 },
    {   36.8218,   -6.87088,    4.59094 },
    {   29.6073,    4.61088,    2.24758 },
    {  -12.9818,   -4.20491,  -0.902244 },
    {   23.7147,   -6.63405,   -1.85474 },
    { -0.553192,   -4.46173,    6.08741 },
    {  -10.8125,   -3.64657,  -0.912971 },
    {  -32.5882,    8.97985,    1.60627 },
    {   22.6064,    10.7259,   -3.23654 },
    {  -9.26259,    8.98537,  0.0169092 },
    {  -3.28397,   -14.1573,   -7.04651 },
    {   9.22003,    12.3861,   -3.42834 },
    {  -25.5849,   -2.78169,   0.386716 },
    {  -26.9032,   -2.93097,    2.44552 }
};


static const std::size_t total_rows = sizeof(row_data)/sizeof(row_data[0]);
static const std::size_t rows4testing = total_rows/10;
static const std::size_t rows_number = total_rows - rows4testing;

inline const mlmodels::training_data& 
    unittest_data()
{
    static const struct generator {
        generator() : data(boost::extents[rows_number][3])
        {
            using  index = mlmodels::training_data::index;
            for (index i = 0; i < (index)rows_number; ++i) {
                for (index j = 0; j < 3; ++j) {
                    data[i][j] = row_data[i][j];
                }
            }
        }

        mlmodels::training_data data;
    } all_data = generator{};

    return all_data.data;
}

inline const mlmodels::class_data& 
    classes4ut() {

        static const struct generator {
            generator() : 
                data(rows_number, mlmodels::class_data::value_type{}) {
                        boost::generate(data, [this]() {
                                auto r = std::rand();
                                return r %  this->classes;    // number of classes
                            }
                        );
            }
            int classes = NUMBER_OF_CLASSES;
            mlmodels::class_data data;
        } generate = generator{};
        return generate.data;
}


inline const  mlmodels::training_data& samples4test()
{
    static const struct generator {
        generator() : data(boost::extents[rows4testing][3])
        {
            using  index = mlmodels::training_data::index;
            index ii = 0;
            for (index i = rows_number; ii < (index)rows4testing; ++i, ++ii) {
                for (index j = 0; j < 3; ++j) {
                    data[ii][j] = row_data[i][j];
                }
            }
        }

        mlmodels::training_data data;
    } data_gen = generator{};
    return data_gen.data;
}

}   // end of namespace ut_data

