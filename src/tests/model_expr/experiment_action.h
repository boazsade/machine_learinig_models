#pragma once
#include <memory>
#include <iostream>

// type erasure holder that can hide that actual implementation 
// and allow using different actions that exposes the same interface
// note that these actions are not related, but only need to be supported
// with the global functions "preform_action" they all 
// defined with input and output types, but the actual action is hidden
template<typename Arg, typename Res>
struct experiment_action
{
    using arg_type = Arg;
    using result_type = Res;

    template<typename T>
    experiment_action(T underlay_data) :
        impl(std::make_shared<object<T>>(std::move(underlay_data)))
    {
    }

    template<typename A, typename R>
    experiment_action<A, R>
    rebind() const
    {
        experiment_action<A, R> result;
        result.impl = impl;
        return result;
    }

    static result_type preform(const experiment_action& self, 
            const arg_type& args) 
    {
        return self.impl->preform(args);
    }
#if 0
    static result_type modify(const experiment_action& self, 
            arg_type& args) 
    {
        return self.impl->preform2(args);
    }
#endif
    static std::ostream& print(const experiment_action& self,
            std::ostream& os)
    {
        return self.impl->print(os);
    }

private:

    struct _placeholder {};

    experiment_action() : 
        impl(std::make_shared<_placeholder>()) 
    {
    }

    struct interface
    {
        virtual ~interface() = default;

        virtual std::ostream& print(std::ostream& os) const = 0;
        virtual result_type preform(const arg_type& args) const = 0;
        //virtual result_type preform2(arg_type& args) const = 0;
    };

    template<typename Underlay>
    struct object : interface
    {
        object(Underlay td) :
            action(std::move(td))
        {
        }

        std::ostream& print(std::ostream& os) const
        {
            return os<<action;
        }

        result_type preform(const arg_type& args) const
        {
            return preform_action(action, args);
        }
#if 0
        result_type preform2(arg_type& args) const
        {
            return mopdify(action, args);
        }
#endif
        Underlay action;
    };

    std::shared_ptr<interface> impl;
};

template<typename A, typename R> inline
R preform_action(const experiment_action<A, R>& with, const A& args)
{
    return experiment_action<A, R>::preform(with, args);
}
#if 0
template<typename A, typename R> inline
R mopdify(const experiment_action<A, R>& with, A& args)
{
    return experiment_action<A, R>::mopdify(with, args);
}
#endif
template<typename A, typename R> inline
std::ostream& operator << (std::ostream& os, const experiment_action<A, R>& action)
{
    return experiment_action<A, R>::print(action, os);
}

