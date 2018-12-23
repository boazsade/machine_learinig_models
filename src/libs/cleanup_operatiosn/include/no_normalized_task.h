#pragma once
#include "no_normalized_conf.h"

normalized_data::result_type 
    preform_action(const _normalized_nothing& mnorm, 
            normalized_data::arg_type arg);
normalized_data::result_type 
    preform_action(const _t_normalization& tnorm, 
            normalized_data::arg_type arg);

