#pragma once
#include <memory>

namespace mlmodels
{
namespace svm
{

struct model_impl;

using model_type = std::shared_ptr<model_impl>;

}   // end of namespace svm
}   // end of namespace mlmodels
