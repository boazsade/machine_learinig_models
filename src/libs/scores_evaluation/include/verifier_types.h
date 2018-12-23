#pragma once
#include <iosfwd>
#include "libs/model_datatypes/model_types.h"

enum verifier_types {
    AUC_ROC_t,
    MSE,
    log_loss,
    kalmogorov_smirnov,
    confusion_matric
};

bool classifier_supported(verifier_types vt);
bool regression_supported(verifier_types vt);
bool is_supported(verifier_types vt, actions::model_type mt);

std::ostream& operator << (std::ostream& os, verifier_types vt);
