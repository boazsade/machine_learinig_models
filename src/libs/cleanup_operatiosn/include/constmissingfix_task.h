#pragma once
#include "constfill_conf.h"
#include "fix_missing.h"

fix_missing::result_type preform_action(const const_value_fix& alog, fix_missing::arg_type arg);
fix_missing::result_type preform_action(const _fix_nothing& alog, fix_missing::arg_type arg);
