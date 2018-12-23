#pragma once
#include "libs/sources_handling/input_data.h"
#include "column_stats.h"
#include <utility>
#include <memory>
#include <vector>

class policy_action
{
public:
    template<typename Action>
    policy_action(Action a) :
        operation(std::make_shared<action<Action>>(std::move(a)))
    {
    }

    static std::pair<bool, input_data> apply(policy_action& pa, input_data arg) {
        return pa.operation->preform(arg);
    }

    static void collect(const policy_action& pa, row_numbers& rn,
            row_numbers& m) {
        pa.operation->collect(rn, m);
    }
    
private:
    struct op_base {
        virtual ~op_base() = default;
        virtual std::pair<bool, input_data> preform(input_data) = 0;
        virtual void collect(row_numbers& rn, row_numbers& m) const = 0;
    };

    template<typename A>
    struct action : op_base {
        action(A a) : 
            data(std::move(a))
        {
        }

        virtual std::pair<bool, input_data> preform(input_data arg) 
        {
            return apply_action(data, arg);
        }

        void collect(row_numbers& rn, row_numbers& m) const
        {
            return finding(data, rn, m);
        }

        A data;
    };

    std::shared_ptr<op_base> operation;
};

using actions_list = std::vector<policy_action>;

std::pair<bool, input_data> apply_action(policy_action& ac, input_data arg);
void finding(const policy_action& pa, row_numbers& rn, row_numbers& m);
