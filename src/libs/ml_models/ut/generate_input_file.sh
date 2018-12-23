#!/usr/bin/env bash

function error_exit {
    echo "critical error : $@"
#    rm -rf ${OUTPUT_FILE}
    exit 1
}

if [ $# -ne 4 ]; then
    echo "usage: <train file> <test file> <predict file> <output file prefix>"
    exit 1
fi

TRAIN_FILE=$1
TEST_FILE=$2
PREDICT_FILE=$3
PREFIX_NAME=$4

if [ ! -f ${TRAIN_FILE} ]; then
    echo "file not found ${TRAIN_FILE}"
    exit 2
fi
if [ ! -f ${TEST_FILE} ]; then
    echo "file not found ${TEST_FILE}"
    exit 2
fi
if [ ! -f ${PREDICT_FILE} ]; then
    echo "file not found ${PREDICT_FILE}"
    exit 2
fi

SAMPLES_TRAIN=$(wc -l ${TRAIN_FILE} | awk '{print $1}') || error_exit "failed read samples count"
SAMPLES_TEST=$(wc -l ${TEST_FILE} | awk '{print $1}') || error_exit "failed read samples count" 
FEATURES_1=$(awk '{print NF}' ${TRAIN_FILE} | sort | uniq) || error_exit "failed to read features count"
FEATURES_COUNT=$(echo "${FEATURES_1} - 1" | bc) || error_exit "failed to read features count"
OUTPUT_FILE=${PREFIX_NAME}_data.h

echo "#pragma once" > ${OUTPUT_FILE}
echo -e "#include \"convertion_op.hpp\" \n#include \"libs/ml_models/general_types.h\"\nnamespace {\n" >> ${OUTPUT_FILE} || error_exit "failed to save to ${OUTPUT_FILE}"


echo "const std::vector<mlmodels::value_type> ${PREFIX_NAME}_predictions {" >> ${OUTPUT_FILE} || error_exit "failed to save to output file"
awk '{print $1 "," }' ${PREDICT_FILE} >> ${OUTPUT_FILE} || error_exit "failed to save prediction data to ${OUTPUT_FILE}"
echo "};" >> ${OUTPUT_FILE}
echo "const mlmodels::class_data ${PREFIX_NAME}_labels {" >>${OUTPUT_FILE} && awk '{print $1 ", "}' ${TRAIN_FILE} >> ${OUTPUT_FILE} && echo "};" >>${OUTPUT_FILE}
echo "const mlmodels::training_data ${PREFIX_NAME}_train_features = make_multi_array<mlmodels::value_type, ${SAMPLES_TRAIN}, ${FEATURES_COUNT}>(" >> ${OUTPUT_FILE} && echo "{" >> ${OUTPUT_FILE}
awk '{ for ( i = 2; i < NF; i++) { printf $i " "}; print $NF }' ${TRAIN_FILE}  | sed 's/:/ /g' | awk  '{ printf "{"; for (i=2; i < NF; i +=2) { printf $i ", "}; print $NF "},"}' >> ${OUTPUT_FILE} || error_exit "failed to save features from ${TEST_FILE}" || error_exit "failed to save features for training"
echo "}" >> ${OUTPUT_FILE} && echo ");" >> ${OUTPUT_FILE}
echo "const mlmodels::training_data ${PREFIX_NAME}_test_features = make_multi_array<mlmodels::value_type, ${SAMPLES_TEST}, ${FEATURES_COUNT}>(" >> ${OUTPUT_FILE} && echo "{" >> ${OUTPUT_FILE}
awk '{ for ( i = 2; i < NF; i++) { printf $i " "}; print $NF }' ${TEST_FILE}  | sed 's/:/ /g' | awk  '{ printf "{"; for (i=2; i < NF; i +=2) { printf $i ", "}; print $NF "},"}' >> ${OUTPUT_FILE} || error_exit "failed to save features from ${TEST_FILE}"
echo "}" >> ${OUTPUT_FILE} && echo ");" >> ${OUTPUT_FILE} && echo "}" >> ${OUTPUT_FILE} || error_exit "failed to finialized output file ${OUTPUT_FILE}"
echo "succesfully generate test file ${OUTPUT_FILE}"
exit 0
