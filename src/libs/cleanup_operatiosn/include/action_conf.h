#pragma once
#include "libs/rows_drops/filterout_conf.h"
#include "libs/rows_drops/drop_unwanted.h"
#include "operetional_pipline.h"
//#include "map_values_op.h"
#include <boost/filesystem.hpp>
#include <iosfwd>
#include <string>

template<typename DropAction>
struct actions_pipeline
{
    using id_type = std::string;
    using drop_action = DropAction;
    using convert_action = fix_missing;
    using normalized_action = normalized_data;
//    using map_val_action = map_value_op;

    actions_pipeline() = default;
    explicit actions_pipeline(const id_type& id) :
        the_id(id)
    {
    }

    actions_pipeline(const id_type& id, drop_action da,
            const convert_action& ca, const normalized_action& na) :
            //const map_val_action& mv) :
        the_id(id), dropact(std::move(da)), 
        fixact(ca),
        normact(na)
        //map_act(mv)
    {
    }

    id_type& id() 
    {
        return the_id;
    }

    const id_type& id() const
    {
        return the_id;
    }

    drop_action& drop()
    {
        return dropact;
    }

    const drop_action& drop() const
    {
        return dropact;
    }

    convert_action& fixes()
    {
        return fixact;
    }

    const convert_action& fixes() const
    {
        return fixact;
    }

    normalized_action& normalization()
    {
        return normact;
    }

    const normalized_action& normalization() const
    {
        return normact;
    }
#if 0
    const map_val_action& map_action() const
    {
        return map_act;
    }

    map_val_action& map_action()
    {
        return map_act;
    }
#endif

private:
    id_type the_id;
    drop_action dropact;
    convert_action fixact;
    normalized_action normact;
    //map_val_action map_act;
};

using preprocessed_action = actions_pipeline<filterout_conf>;
using column_actions = actions_pipeline<drop_unwanted>;

const preprocessed_action::id_type& id(const preprocessed_action& apl);
const column_actions::id_type& id(const column_actions& ca);
boost::filesystem::path data_location(const column_actions& ca);

bool operator == (const preprocessed_action& left, const preprocessed_action::id_type& right);
bool operator != (const preprocessed_action& left, const preprocessed_action::id_type& right);
bool operator == (const preprocessed_action::id_type& left, const preprocessed_action& right);
bool operator != (const preprocessed_action::id_type& left, const preprocessed_action& right);
bool operator == (const column_actions& left, const column_actions::id_type& right);
bool operator != (const column_actions& left, const column_actions::id_type& right);
bool operator == (const column_actions::id_type& left, const column_actions& right);
bool operator != (const column_actions::id_type& left, const column_actions& right);

std::ostream& operator << (std::ostream& os, const column_actions& ca);
std::ostream& operator << (std::ostream& os, const preprocessed_action& ca);

