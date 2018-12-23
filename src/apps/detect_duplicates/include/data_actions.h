#pragma once
#include "libs/sources_handling/input_data.h"
#include <string>
#include <memory>

class data_action
{
    template<typename Action>
    data_action(Action&& a) :
        operation(std::make_shared<action<Action>>(std::move(a)))
    {
    }

    friend bool apply_action(const data_action& pa, const input_data& arg) {
        return pa.operation->preform(arg);
    }
    
private:
    struct op_base {
        virtual ~op_base() = default;
        virtual bool preform(const input_data&) const = 0;
    };

    template<typename A>
    struct action : op_base {
        action(A a) : 
            data(std::move(a))
        {
        }

        virtual bool preform(const input_data& arg) const
        {
            return apply_action(data, arg);
        }

        A data;
    };

    std::shared_ptr<const op_base> operation;
};

using actions_list = std::vector<policy_action>;
