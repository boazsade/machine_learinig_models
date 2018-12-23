#include "apps/run_experiments/actions_pipeline.h"

extract4training::result_type extract4training::operator () (const arg_type& a) const
{
    return preform_action(action, a);
}

std::ostream& extract4training::print(std::ostream& os) const
{
    return os<<action;
}

extract4testing ::result_type extract4testing::operator () (const arg_type& a) const
{
    return preform_action(action, a);
}

std::ostream& extract4testing::print(std::ostream& os) const
{
    return os<<action;
}

model_tester::result_type model_tester::operator () (const arg_type& a) const
{
    return preform_action(action, a);
}

std::ostream& model_tester::print(std::ostream& os) const
{
    return os<<action;
}

model_generator::result_type model_generator::operator () (const arg_type& a) const
{
    return preform_action(action, a);
}

std::ostream& model_generator::print(std::ostream& os) const
{
    return os<<action;
}

extract4training::result_type
    preform_action(const extract4training& with, 
            const extract4training::arg_type& args)
{
    return with(args);
}
// 2. generate model
model_generator::result_type
    preform_action(const model_generator& with,
            const model_generator::arg_type& args)
{
    return with(args);
}
// 3. exterat datga for testing
extract4testing::result_type 
    preform_action(const extract4testing& with,
            const extract4testing::arg_type& args)
{
    return with(args);
}

// 4. test model
model_tester::result_type
    preform_action(const model_tester& with, 
            const model_tester::arg_type& args)
{
    return with(args);
}

std::ostream& operator << (std::ostream& os, const model_tester& what)
{
    return what.print(os);
}

std::ostream& operator << (std::ostream& os, const model_generator& what)
{
    return what.print(os);
}

std::ostream& operator << (std::ostream& os, const extract4training& what)
{
    return what.print(os);
}

std::ostream& operator << (std::ostream& os, const extract4testing& what)
{
    return what.print(os);
}

