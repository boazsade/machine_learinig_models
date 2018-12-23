#include "libs/scores_evaluation/score_evaluation.h"
#include "libs/experiments/ROC.h"
#include "libs/experiments/mse.h"
#include "libs/experiments/kolmogorov_smironov_test.h"
#include "libs/experiments/confusion_matric.h"
#include <boost/iterator/iterator_facade.hpp>

namespace
{

struct measure_pred
{
    exprs::value_type measure = 0.f;
    exprs::value_type prediction = 0.f;

    constexpr measure_pred() = default;
    constexpr measure_pred(exprs::value_type m, exprs::value_type p) :
            measure{m}, prediction{p}
    {
    }
};

class exprs_iterator : public boost::iterator_facade<
                                exprs_iterator, const measure_pred,
                                boost::random_access_traversal_tag,
                                measure_pred>
{
public:
    explicit exprs_iterator(difference_type max) :
        preds{nullptr}, meas{nullptr}, current_location{max}
    {
    }

    exprs_iterator(const actions::experiment_input& input) :
        preds{&actions::predictions(input)},
        meas{&actions::measures(input)},
        current_location{0}
    {
    }

private:
    friend class boost::iterator_core_access;
    void decrement()
    {
        if (preds && meas) {
            --current_location;
        }
    }

    void increment()
    {
        if (meas && preds) {
            ++current_location;
        }
    }

    void advance(difference_type n)
    {
        if (meas && preds) {
            current_location += n;
        }
    }

    bool equal(exprs_iterator const& other) const
    {
        return other.current_location == current_location;
    }

    measure_pred dereference() const
    {
        if (preds && meas) {
            return measure_pred{(*meas)[current_location],
                (*preds)[current_location]
            };
        } else {
            return measure_pred{std::numeric_limits<exprs::value_type>::max(),
                                std::numeric_limits<exprs::value_type>::max()
            };
        }
    }

    using prediction_t = std::tuple_element<actions::expers_index::PREDICTIONS, actions::experiment_input>::type;
    using measure_t = std::tuple_element<actions::expers_index::MEASURE, actions::experiment_input>::type;

    const prediction_t* preds = nullptr;
    const measure_t* meas = nullptr;
    difference_type current_location = -1;
};


}   // end of namesapce

AUC_ROC::result_type
    calculate(const AUC_ROC& , 
            const actions::experiment_input& input)
{
    //return actions::testing_output{1, 2, 3, 4};
    exprs::ROC calculator;
    if (calculator.calculate(std::get<1>(input), // this is the actual data that we have from the source
                std::get<0>(input))) {                   // and this is the data from the prediction of the model
        return calculator.results;
    } else {
        throw std::runtime_error{"failed to calcualte ROC/AOC"};
    }

}

null_verifier::result_type
    calculate(const null_verifier& , const actions::experiment_input&)
{
    return null_verifier::result_type{};
}

logistic_loss::result_type
    calculate(const logistic_loss& , 
            const actions::experiment_input&)
{
    //return actions::testing_output{45.6, 345.66, 123.55, 234.672534};
    return logistic_loss::result_type{};
}

kalmogorov_smirnov_test::result_type
    calculate(const kalmogorov_smirnov_test& , 
            const actions::experiment_input& input)
{

    auto data = std::get<0>(input);
    auto measure = std::get<1>(input);
    return exprs::ks::accuracy_test(data, measure);
}

MSE_test::result_type
    calculate(const MSE_test&, const actions::experiment_input& input)
{
    //using list_res = actions::experiment_output::list_items::value_type;
    return exprs::mse(std::get<1>(input), std::get<0>(input));
}

confusion_matric_test::result_type
    calculate(const confusion_matric_test& , const actions::experiment_input& input)
{
    confusion_matric_test::result_type res;
    res = std::accumulate(exprs_iterator{input}, exprs_iterator{(exprs_iterator::difference_type)actions::predictions(input).size()}, 
            res, [](const auto& target, auto data) {
                return target + exprs::binclass::cm::calculate(data.measure, data.prediction);
            }
    );
    return res;
}

