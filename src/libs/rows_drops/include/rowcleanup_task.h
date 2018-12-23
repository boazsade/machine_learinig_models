#pragma once
#include "filterout_conf.h"
#include "drop_unwanted.h"

drop_unwanted::result_type preform_action(const filterout_conf& alog, drop_unwanted::arg_type arg);
drop_unwanted::result_type preform_action(const _dont_drop& alog, drop_unwanted::arg_type arg);
