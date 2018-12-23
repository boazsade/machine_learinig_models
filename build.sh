APPS='data_clean detect_duplicates csv_info'
APPS_DIR=src/apps
BUILD_LOG=/tmp/build_log
UNIT_TEST_LOG=/tmp/unit_test_log

function die_ {
    echo "$@"
    exit 1
}


for app_name in $APPS; do
    make -C ${APPS_DIR}/${app_name} > ${BUILD_LOG} 2>&1 || die_ "failed to build application " $app_name
    make -C ${APPS_DIR}/${app_name} > ${UNIT_TEST_LOG} 2>&1 || die_ "unit tests for " $app_name "failed"
done
