#include "libs/experiments/ROC.h"
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <math.h>
#include <utility>
#include <tuple>
#include <iostream>
#include <iterator>
#include  <stdexcept>

namespace exprs
{

value_type accuracy(value_type threshold, const std::vector<int>& true_val, 
        const data_array& pred)
{
    static const value_type eps = 1.0e-10;
    int a  = 0,b = 0,c = 0, d = 0;
    for (int item = 0; item < (int)true_val.size(); item++) {
        if (true_val[item] == 1) {
            if (pred[item] >= threshold) {
                a++;
            } else {
                b++;
            }
        } else {
            if (pred[item] >= threshold) {
                c++;
            } else {
                d++;
            }
        }
    }
    return( ((value_type)(a+d)) / (((value_type)(a+b+c+d)) + eps) );
}

int partition(int p, int r, std::vector<int>& true_val, data_array& pred)
{
    
    value_type x = pred[p];
    if (p >= (int)pred.size() || p < 0) {
        throw std::runtime_error{"cannot partition lower limit - out of range between " + std::to_string(pred.size()) + 
            " and " + std::to_string(p)};
    }
    if (r <= 0 || r >= (int)pred.size()) {
       throw std::runtime_error{"cannot partition upper limit - out of range between " + std::to_string(r) +
                " and " + std::to_string(pred.size())};
    } 
    int i = p - 1;
    int j = r + 1;
    while (true) {
        do j--; while (j > -1 && pred[j] > x);
        if (j < 0) {
            throw std::runtime_error{"invalid value given no value less than " + std::to_string(x) + " found"};
        }
        do i++; while (i < (int)pred.size() && pred[i] < x );
        if (i == (int)pred.size()) {
            throw std::runtime_error{"invalid value given no value greater than " + std::to_string(x) + " found"};
        }
        if (i < j) {
            std::swap(pred[i], pred[j]);
            std::swap(true_val[i], true_val[j]);
        } else {
            return j;
        }
    }
}

void quicksort(int p, int r, std::vector<int>& true_val, data_array& pred)
{
    if (p < r) {
        int q = partition(p, r, true_val, pred);
        quicksort(p, q, true_val, pred);
        quicksort(q+1, r, true_val, pred);
    }
}

template<typename T>
value_type calculateRmse(const std::vector<T>& true_val, const data_array& pred,
        value_type& mean_true)
{
    value_type sse = 0.0;
    for(int no_item = 0; no_item < (int)true_val.size(); ++no_item) {
        value_type p1 = pred[no_item];
        sse+= (true_val[no_item]-p1)*(true_val[no_item]-p1);
        mean_true += true_val[no_item];
    }
    mean_true /= (value_type) true_val.size();
    return  sqrt(sse / ((value_type)true_val.size()));
    
}
std::tuple<value_type, value_type, 
    data_array, data_array>
do_calculation(ROC::algo_params&& calc_params)
{
    // now let's do the ROC cruve and area 
    quicksort(0, (int)(calc_params.partitions.size() - 1u), calc_params.partitions, calc_params.predictions);
    auto total_0 = calc_params.predictions.size() - calc_params.condition_pos;
    auto tt = 0;
    auto tf = calc_params.condition_pos;
    auto ft = 0;
    auto ff = total_0;
    
    auto sens = ((value_type) tt) / ((value_type) (tt+tf));
    auto spec = ((value_type) ff) / ((value_type) (ft+ff));
    auto tpf = sens;
    auto fpf = 1.f - spec;
    data_array true_pf, false_pf;
    
    true_pf.push_back(tpf);
    false_pf.push_back(fpf);
    auto roc_area = 0.f;
    auto tpf_prev = tpf;
    auto fpf_prev = fpf;
    
    auto no_item = calc_params.predictions.size();
    for (int item=no_item-1; item>-1; item--) {
        tt+= calc_params.partitions[item];
        tf-= calc_params.partitions[item];
        ft+= 1 - calc_params.partitions[item];
        ff-= 1 - calc_params.partitions[item];
        sens = ((value_type) tt) / ((value_type) (tt+tf));
        spec = ((value_type) ff) / ((value_type) (ft+ff));
        tpf  = sens;
        fpf  = 1.f - spec;
        if (item > 0) {
            if (calc_params.predictions[item] != calc_params.predictions[item - 1]) {
                true_pf.push_back(tpf);
                false_pf.push_back(fpf);
                roc_area += 0.5f * (tpf + tpf_prev) * (fpf-fpf_prev);
                tpf_prev = tpf;
                fpf_prev = fpf;
            }
        }
        if (item == 0) {
            true_pf.push_back(tpf);
            false_pf.push_back(fpf);
            roc_area += 0.5f * (tpf+tpf_prev) * (fpf-fpf_prev);
        }
    } 
    auto acc = accuracy(0.5, calc_params.partitions, calc_params.predictions);
    return std::make_tuple(roc_area, acc, std::move(true_pf), std::move(false_pf));
}

template<typename T>
std::size_t ROC::algo_params::init(const std::vector<T>& ex,
        std::vector<int>& target,
        value_type threshold)
{
    auto pos = 0u;
    boost::transform(ex, std::back_inserter(target), [&pos, threshold](auto val) {
                if (val < threshold) {
                    return 0;
                } else {
                    ++pos;
                    return 1;
                }
            }
    );
    return pos;
}

ROC::algo_params::algo_params(const data_array& ex, const data_array& predic,
        value_type threshold) :
        predictions(predic)/*, partitions(ex.size(), 0)*/
{
    condition_pos = init(ex, partitions, threshold);
}

ROC::algo_params::algo_params(const std::vector<int>& ex, 
        const data_array& p, value_type threshold) :
        predictions(p)/*, partitions(ex.size(), 0)*/
{
    condition_pos = init(ex, partitions, threshold);
}

template<typename T>
bool ROC::calc(const std::vector<T>& expected,
            const data_array& predict)
{
    if (!expected.empty() && expected.size() == predict.size()) {
        value_type mean = 0.f;
        results.rmse = calculateRmse(expected, predict, mean);
        algo_params data4calc{expected, predict, mean};
        if (data4calc.condition_pos != 0) {
            std::tie(results.area, results.accuracy, results.sensitivity, results.fall_out) = 
                do_calculation(std::move(data4calc));
            return !results.sensitivity.empty();
        }
    }
    return false;
}

bool ROC::calculate(const data_array& expected,
                    const data_array& predict)
{
    return calc(expected, predict);
}

bool ROC::classes_calculate(const std::vector<int>& expected,
                const data_array& predict)
{
    return calc(expected, predict);
}

bool ROC::classes_calculate(const data_array& expected,
                const data_array& predict)
{
    return calc(expected, predict);
}

std::ostream& operator << (std::ostream& os, const ROC& roc)
{
    os<<"area: "<<std::fixed<<roc.results.area<<" RMS error: "<<roc.results.rmse<<
        " accuracy: "<<roc.results.accuracy;
    os<<"sensitivity   fall_out\n";
    boost::transform(roc.results.sensitivity, roc.results.fall_out, 
                    std::ostream_iterator<std::string>(os, "\n"), [](auto s, auto f) {
                        return std::to_string(s) + "      " + std::to_string(f);
                    }
            );
    return os;
}

}   // end of namespace exprs

