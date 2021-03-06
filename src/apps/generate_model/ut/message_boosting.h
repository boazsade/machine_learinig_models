#pragma once
namespace {
const char* BOOSTING_MESSAGE = "{"
    "\"dataset\": \"59705aa8aa39d855e5678b50\","
    "\"datasource\": {"
            "\"path\": \"/home/boaz/work/machine_learinig_models/src/apps/run_experiments/ut/input.csv\""
    "},"
    "\"model\": {"
        "\"class\": \"five\","
        "\"type\": \"classifier\","
        "\"repeations\": 1"
    "},"
    "\"action\": {"
            "\"name\": \"model.boosting\","
            "\"attr\": {"
                "\"max-categories\": 10,"
                "\"cv-fold\": 1,"
                "\"max-depth\":-1,"
                "\"min-samples\":10,"
                "\"use-surrogates\":false,"
               " \"flags\": \"auto\","
                "\"weak-count\": 100,"
                "\"type\": \"real\","
                "\"weight-trim-rate\": 0.97"
            "}"
    "},"
    "\"evaluation\": ["
        "\"auc-roc\", \"ks-test\""
    "],"
    "\"export\": {"
        "\"type\": \"yml\","
        "\"path\": \"/home/boaz/work/machine_learinig_models/src/apps/run_experiments/ut/model.yml\""
    "},"
    "\"validation\": {"
        "\"action\": \"precent\","
        "\"attr\": {"
            "\"arg\": 80"
        "}"
    "}"
"}";
}
