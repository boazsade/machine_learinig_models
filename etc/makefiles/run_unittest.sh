#!/usr/bin/env bash
if [ $# -ne 3 ]; then
    echo "failed to run unit test missing args"
fi
export LC_ALL="en_US.UTF-8"
UNIT_TEST_NAME=$1
UNIT_TEST_DIR=$2
GCC_SO_DIR=$3

export LD_LIBRARY_PATH=${GCC_SO_DIR}
${UNIT_TEST_DIR}/${UNIT_TEST_NAME}
exit $?
