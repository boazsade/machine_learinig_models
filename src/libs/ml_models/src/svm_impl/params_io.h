#pragma once
#include <iosfwd>

struct svm_parameter;

const char* type2string(int type);
const char* kernel2string(int kernel);
std::ostream& operator << (std::ostream& os, const svm_parameter& sp);
