#include "libs/scores_evaluation/verifier_types.h"
#include <iostream>


bool classifier_supported(verifier_types vt)
{
    switch (vt) {
        case kalmogorov_smirnov:
        case AUC_ROC_t:
        case confusion_matric:
        case MSE:
            return true;
        default:
            return false;
    }
}

bool regression_supported(verifier_types vt)
{
    switch (vt) {
        case kalmogorov_smirnov:
        case AUC_ROC_t:
        case MSE:
            return true;
        default:
            return false;
    }
}

bool is_supported(verifier_types vt, actions::model_type mt)
{
    switch (mt) {
        case actions::CLASSIFIER_MODEL:
            return classifier_supported(vt);
        case actions::REGRESSION_MODEL:
            return regression_supported(vt);
        default:
            return false;
    }
}

std::ostream& operator << (std::ostream& os, verifier_types vt)
{
    switch (vt) {
        case kalmogorov_smirnov:
            return os<<"kalmogorov smirnov";
        case AUC_ROC_t:
            return os<<"AUC ROC";
        case log_loss:
            return os<<"logistic loss";
        case MSE:
            return os<<"MSE";
        case confusion_matric:
            return os<<"confusion_matric";
    }

}

