#pragma once
#include "constmissingfix_task.h"
#include "fillinvalid_conf.h"
#include "fix_missing.h"

fix_missing::result_type preform_action(const cubical_fix& cubic_alog, fix_missing::arg_type arg);
fix_missing::result_type preform_action(const linear_fix& linear_alog, fix_missing::arg_type arg);
fix_missing::result_type preform_action(const kneighborhood_fix& alog, fix_missing::arg_type arg);

