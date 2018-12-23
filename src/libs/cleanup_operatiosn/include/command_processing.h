#pragma once
#include "command_processing_data.h"
#include "dataclean_conf.h"
#include "schema_data.h"
#include <string>

dataclean_conf_preliminery read_configuration(const std::string& input);

dataclean_conf finilized_configuration(dataclean_conf_preliminery&& prem_conf, 
        const schema_type& schema);

dataclean_conf process_configuration(const std::string& input);

