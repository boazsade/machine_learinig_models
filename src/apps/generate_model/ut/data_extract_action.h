#include "apps/generate_model/processing_pipeline.h"
#include <string>

#pragma once

namespace flowut
{

inline 
pipeline::config::source_policy make_read_policy(unsigned int size, const std::string& id) {
        return pipeline::config::source_policy{
                    pipeline::config::source_policy{
                                    percent_split{size, id}
                } // the name of class column
        };
}

}   // end of namespace flowut
