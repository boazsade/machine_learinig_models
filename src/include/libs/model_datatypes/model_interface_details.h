#pragma once
//#include "model_save_function.h"
#include <boost/filesystem.hpp>
#include <tuple>
#include <utility>
#include  <string>


namespace detail
{

///////////////////////////////////////////////////////////////////////////////
//  saving
template<typename Impl>
struct saving_model// : save_define
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = boost::filesystem::path;
    using result_type = bool;
    
    saving_model(model_type m) :
        model{std::move(m)}
    {
    }

    result_type execute(const input_type& to) const
    {
        if (boost::filesystem::exists(to.parent_path())) {
            model_impl mr;
            return mr.save(model, to.string());
        } else {
            return false;
        }
    }

private:
    model_type model;
};

template<typename Impl> inline 
std::string to_string(const saving_model<Impl>&) {
    return "save";
}

///////////////////////////////////////////////////////////////////////////////
//  testing
template<typename Impl>
struct testing_model
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = typename Impl::testing_input;
    using result_type = typename Impl::test_output;

    testing_model(model_type m,
            int cls) :
        model{std::move(m)},
        class_count{cls}
    {
    }

    result_type execute(const input_type& input) const
    {
        Impl mr;
        return mr.test(model, input, class_count);
    }

    template<typename To>
    static To do_cast(const testing_model<Impl>& from) {
            return To(from.model);
        }

private:
    model_type model;
    int class_count = -1;
};

template<typename Impl> inline 
std::string to_string(const testing_model<Impl>&) {
    return "test";
}

template<typename To, typename Impl> inline
To testing_cast(const testing_model<Impl>& from) {
        return To{
            testing_model<Impl>::template do_cast<To>(from)
    };
}

///////////////////////////////////////////////////////////////////////////////
//  training 
template<typename Impl, typename TrainRes>
struct training_impl
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = typename Impl::training_data;
    using model_args = typename Impl::args;
    using labels_data = typename Impl::labels_type;
    using result_type = TrainRes;

    training_impl() = default;
    training_impl(model_args a) :
        args(std::move(a))
    {
    }

    result_type execute(const input_type& input,
            const labels_data& labels,
            int class_num) const
    {
        Impl mr;
        auto model = mr.train(input, labels, args);
        return result_type{std::move(model), class_num};
    }

private:
    model_args args;
};

template<typename T, typename F> inline 
std::string to_string(const training_impl<T, F>&) {
    return "training";
}

///////////////////////////////////////////////////////////////////////////////
//  predict
template<typename Impl>
struct predict_model
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = typename Impl::row_type;
    using result_type = typename Impl::prediction_value;

    predict_model(model_type m) :
                    model{std::move(m)}
    {
    }

    result_type execute(const input_type& input) const
    {
        result_type result;
        model_impl mi;
        return mi.predict(model, input);
    }

private:
    model_type model;
};

template<typename Impl> inline 
std::string to_string(const predict_model<Impl>&) {
    return "predict";
}

///////////////////////////////////////////////////////////////////////////////
//  load
template<typename Impl, typename Target>
struct loading_model
{
    using model_impl = Impl;
    using model_type = typename Impl::model_type;
    using input_type = boost::filesystem::path;
    using result_type = Target;
    
    loading_model() = default;

    result_type execute(const input_type& from) const
    {
        using model_t = typename model_impl::model;
        if (boost::filesystem::exists(from.parent_path())) {
             return result_type{model_t::load(from.c_str())};
        } else {
            return result_type{model_type{}};
        }
    }
};

template<typename Impl, typename Target> inline 
std::string to_string(const loading_model<Impl, Target>&) {
    return "load";
}

}   // end of namesace detail

