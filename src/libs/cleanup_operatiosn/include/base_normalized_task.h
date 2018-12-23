#pragma once
#include "operetional_pipline.h"
#include "base_normalized_conf.h"
#include "no_normalized_task.h"

normalized_data::result_type 
    preform_action(const z_normaliztion_conf& znorm, 
            normalized_data::arg_type arg);
normalized_data::result_type 
    preform_action(const minmax_normalization& minmax_norm, 
            normalized_data::arg_type arg);
normalized_data::result_type 
    preform_action(const max_normalization& mnorm, 
            normalized_data::arg_type arg);
