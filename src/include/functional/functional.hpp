#pragma once
#include <memory>
#include <iostream>
#include <cassert>

// type erasure holder that can hide the actual implementation 
// and allow using different actions that exposes the same interface
// note that these actions are not related, but only need to be supported
// with the global functions "preform_action" they all 
// defined with input and output types, but the actual action is hidden
// note that for generic invokation (not when we need to store something),
// there is an invake and apply in the standard C++ 17 and onward, that togheter 
// with lambda can achive the same things, only problem with these is that you
// cannot build a sort of pipeline of operations that doing different things internally
// but look uniform from the outside
template<typename Arg, typename Res>
struct poli_function
{
    using arg_type = Arg;
    using result_type = Res;

    poli_function() = default;

    template<typename T>
    poli_function(T underlay_data) :
        impl(std::make_shared<object<T>>(std::move(underlay_data)))
    {
    }

    template<typename T>
    poli_function& operator = (T fi) 
    {
        impl = std::make_shared<object<T>>(std::move(fi));
        return *this;
    }

    static result_type preform(const poli_function& self, 
            const arg_type& args) 
    {
        return self.impl->preform(args);
    }

    static std::ostream& print(const poli_function& self,
            std::ostream& os)
    {
        assert(self.impl);
        return self.impl->print(os);
    }

    template<typename A, typename R>
    poli_function<A, R>
    rebind() const
    {
        poli_function<A, R> result{impl};
        return result;
    }

    static std::string as_string(const poli_function& me)
    {
        return me.impl->as_string();
    }

private:

    struct interface
    {
        virtual ~interface() = default;

        virtual std::ostream& print(std::ostream& os) const = 0;
        virtual result_type preform(const arg_type& args) const = 0;
        virtual std::string as_string() const = 0;
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

        std::string as_string() const 
        {
            return to_string(action);
        }

        Underlay action;
    };

    using impl_type = std::shared_ptr<interface>; 

    poli_function(impl_type ip) :
        impl{ip}
    {
    }

    impl_type impl;
};

template<typename A, typename R> inline
R preform_action(const poli_function<A, R>& with, const A& args)
{
    return poli_function<A, R>::preform(with, args);
}

template<typename A, typename R> inline
std::ostream& operator << (std::ostream& os, const poli_function<A, R>& action)
{
    return poli_function<A, R>::print(action, os);
}

template<typename A1, typename R1, typename A2, typename R2>
inline poli_function<A1, R1> rebind(poli_function<A2, R2> from) 
{
    return from.template rebind<A1, R1>();
}

template<typename A, typename R> inline
std::string to_string(const poli_function<A, R>& action) {
    return poli_function<A, R>::as_string(action);
}

