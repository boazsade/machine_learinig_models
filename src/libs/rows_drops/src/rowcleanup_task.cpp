#include "libs/rows_drops/rowcleanup_task.h"
#include "logging.h"
#include <algorithm>


drop_unwanted::result_type preform_action(const filterout_conf& algo, drop_unwanted::arg_type arg)
{
    LOG_DEBUG<<"processing input data of size "<<size(arg)<<" by "<<algo;
    auto i = std::remove_if(std::begin(arg), std::end(arg), [&algo] (auto entry) {
                auto it = std::find_if(std::begin(algo.to_remove), 
                                      std::end(algo.to_remove), 
                                      [entry](auto rn) {
                                        return rn == entry.row;
                                      }
                                );
                return it != std::end(algo.to_remove);
            }
    );
    return drop_unwanted::result_type{arg.id(), arg.begin(), i};
}

drop_unwanted::result_type preform_action(const _dont_drop& , drop_unwanted::arg_type arg)
{
    LOG_DEBUG<<"no drop action was declared for column";
    return arg;
}
