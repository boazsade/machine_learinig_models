#include "libs/cleanup_operatiosn/dataclean_conf.h"
#include "libs/cleanup_operatiosn/command_processing_data.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include "logging.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <set>

namespace
{

    template<typename It>
    drop_unwanted make_drop_action(const std::string& id, It rowbeg, It rowend)
    {
        return drop_unwanted(drop_unwanted(filterout_conf(id, rowbeg, rowend)));
    }

    column_actions make_actions(const column_actions::id_type& id, drop_unwanted du,
                        dataclean_conf_preliminery::const_actions_iterator in_act_be,
                        dataclean_conf_preliminery::const_actions_iterator in_act_end)
    {
        auto i = std::find(in_act_be, in_act_end, id);
        if (i != in_act_end) {
            const auto fa = i->fixes();
            const auto na = i->normalization();
            //const auto mv = i->map_action();
            return dataclean_conf::actions_pipline{id, du, fa, na};
        } else {
            static const column_actions::normalized_action defnorm = column_actions::normalized_action{};
            static const column_actions::convert_action deffix = column_actions::convert_action{};
            //static const column_actions::map_val_action mv = column_actions::map_val_action{};
            return dataclean_conf::actions_pipline{id, du, deffix, defnorm};
        }
    }
                        

}   // end of local namespace

bool good(const dataclean_conf& dc)
{
    return !(dc.export_file.empty() || dc.source_location.empty());
}

std::ostream& operator << (std::ostream& os, const dataclean_conf& df)
{
    os<<"source location: ["<<df.source_location<<"], export file: ["<<df.export_file<<
        "]\ndrop actions: ";
    std::copy(begin(df), end(df), std::ostream_iterator<dataclean_conf::actions_pipline>(os, "\n"));
   return os; 
}

dataclean_conf::dataclean_conf(dataclean_conf_preliminery&& from,
                        const schema_type& sc) : 
    source_location(std::move(from.source_location)),
    export_file(std::move(from.export_file)),
    has_actions{not from.actions.empty()}
{

    using unique_rows = std::set<filterout_conf::row_numbers::value_type>;
    unique_rows rows2remove;
    // are are collecting all row number that needs to be droped from all algorithms
    std::for_each(std::begin(from.actions), std::end(from.actions), [&rows2remove] (auto& da) {
                rows2remove.insert(std::begin(da.drop().to_remove), std::end(da.drop().to_remove));
            }
    );
    // we would create new drop rule if one does not exists, or update 
    // what we have
        
    auto fb = std::begin(from.actions);
    auto fe = std::end(from.actions);

    std::transform(std::begin(sc), std::end(sc), 
            std::back_inserter(this->actions), 
            [this, &rows2remove, fb, fe](auto& da) {
                return make_actions(da,
                    make_drop_action(da, rows2remove.begin(), rows2remove.end()),
                    fb, fe
                );
            }
    );
}

const dataclean_conf::actions_pipline& dataclean_conf::operator [] (const id_type& id) const
{
    auto i = std::find(begin(*this), end(*this), id);
    if (i != end(*this)) {
        return *i;
    } else {
        static const actions_pipline invalid = actions_pipline{};
        return invalid;
    }
}

dataclean_conf generate(dataclean_conf_preliminery&& from, const schema_type& sc)
{
    return dataclean_conf(std::move(from), sc);
}

bool empty(const dataclean_conf& dc)
{
    return !good(dc) || dc.actions.empty();
}

std::size_t size(const dataclean_conf& dc)
{
    return dc.actions.size();
}

dataclean_conf::actions_iterator begin(dataclean_conf& dc)
{
    return std::begin(dc.actions);
}

dataclean_conf::actions_iterator end(dataclean_conf& dc)
{
    return std::end(dc.actions);
}

dataclean_conf::const_actions_iterator begin(const dataclean_conf& dc)
{
    return std::begin(dc.actions);
}

dataclean_conf::const_actions_iterator end(const dataclean_conf& dc)
{
    return std::end(dc.actions);
}


