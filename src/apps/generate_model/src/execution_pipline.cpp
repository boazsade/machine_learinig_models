#include "execution_pipline.hpp"
/////////////////////////////////////////////////////////////////////
namespace pipeline
{

read_step operator | 
    (const config& conf, _do_start)
{
    return read_step(conf);
}

read_step::result_type operator | 
    (const read_step& rs, _do_read)
{
    return read(rs);
}

train_data operator | 
    (const read_step& rs, _prepare_it)
{
    return train_data(rs);
}

train_step operator | 
    (const train_data& td, _initial_model)
{
    return train_step(td);
}

test_step operator | 
    (const train_arg_f& ts, const read_step::result_type& input)
{
    return ts(input);
}

experiments_range operator | 
    (const test_output& ts, _expers)
{
    return std::make_tuple(ts.begin(), ts.end());
}


train_arg_f operator |
    (const train_step& ts, const lookup::lookuptable_transform& lt)
{
    return [&ts, &lt](const read_step::result_type& input) { return train(ts, input, lt); };
}

test_arg_f operator |
    (const test_step& ts, const lookup::lookuptable_transform& lt)
{
    return [&ts, &lt](const read_step::result_type& input) { return test(ts, input, lt); };
}

test_output operator | 
    (const test_arg_f& ts, const read_step::result_type& input)
{
    return ts(input);
}

params_output operator | 
    ( const test_output& input, experiments_range exprs)
{
    params_output output;
    const auto& data = expers_data_cast(input);
    output = std::accumulate(std::get<0>(exprs), std::get<1>(exprs), output, [&data](const params_output& po, auto exp) {
                auto tmp =  experiment(exp, data) + po;
                return tmp;
            }
    );
    return output;
}

lookup::lookuptable_transform
    operator | (const config& conf, 
            const classifier::read_result& data)
{
    const  lookup::map_value_op table_gen {conf.map_config};
    return preform_action(table_gen, data);
}

save_step operator | 
    (const test_output& ts, _prep_save)
{
    return save_cast(ts);
}

save_step::result_type operator | 
    (const save_step& s, _do_save)
{
    return save(s);
}


std::string execute(const config& conf)
{
    try {
        auto rs = conf | do_start;      // ready
        auto data = rs | do_read;       // read the data
        auto const tt = conf | data;    // build lookup table to use for processing
        LOG_INFO<<"lookup table transform: "<<tt;
        auto tst = rs | prepare_it | initial_model | tt  | data ;  // build model
        LOG_INFO<<"the test is done on "<<tst;
        auto to = tst | tt | data;
        LOG_INFO<<"we would using "<<to<<" to generate the best model";
        auto ei = tst | tt | data | (to | experiments);         // run experiments
        LOG_INFO<<"finish iteration with results "<<ei;     // show results
        // note that we would need to fix that later to support loop here!!
        auto b = to | prepare_save | do_save;                // and save them
        if (b) {
            return to_string(tt);
        }  
    } catch (const std::exception& e) {
        LOG_ERROR<<"failed to process: "<<e.what();
        throw e;
    } catch (...) {
        LOG_ERROR<<"unknown error while processing data - cannot save model";
        throw std::runtime_error{"uknown error while trying to build model and save it"};
    }

    return std::string{};
}

}   // end of namespace pipeline

