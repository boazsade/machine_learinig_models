#!/usr/bin/env bash

VERSION_NUMBER=__NOT_SET_VERSION_NUMBER_

BASE_DIRECTORY=/opt
INSTALL_DIRCTORY=binah
TARGET_DIRECTORY=${BASE_DIRECTORY}/${INSTALL_DIRCTORY}
LINK_DIRECTORY=${TARGET_DIRECTORY}/backend
BACK_END_DIR=${TARGET_DIRECTORY}/backend_${VERSION_NUMBER}
FRONT_END_DIR=${TARGET_DIRECTORY}/frontend
BACK_END_LOG_DIR=${BACK_END_DIR}/logs
BACK_END_TMP_DIR=${BACK_END_DIR}/tmp
BACK_END_BIN_DIR=${BACK_END_DIR}/bin
SOURCE_PREP_EXE=csv_info
DUP_DETECT_EXE=detect_duplicates
DATA_CLEAN_EXE=data_clean
EXPERS_EXE=run_experiments
CREATE_MODEL=generate_model
MODEL_PRED=model_prediction
DATA_PREP_EXE=*.py

# if we are root, then we dont really care for sudo anyways
if [[ $EUID -ne 0 ]]; then
    HAS_SUDO=$(which sudo > /dev/null 2>&1; echo $?)
    if [ "$HAS_SUDO" = "0" ]; then
        SUDO="`which sudo`"
    fi
fi

function exit_error {
    echo -e "error: $@"
    ${SUDO} rm -rf ${BACK_END_DIR}
    exit 1
}

python --version >/dev/null 2>&1 || exit_error "you must install python before you can continue"
if [ -d ${LINK_DIRECTORY} ]; then
    if [ -L ${LINK_DIRECTORY} ]; then
        ${SUDO}  unlink ${LINK_DIRECTORY}
        echo "removed old link ${LINK_DIRECTORY}"
    else
        ${SUDO}  rm -rf ${LINK_DIRECTORY}
        echo removed old ${LINK_DIRECTORY} as this needs to be link and not directory
    fi
fi

${SUDO}  mkdir -p ${TARGET_DIRECTORY} || exit_error "failed to create directory ${TARGET_DIRECTORY}"
${SUDO}  mkdir -p ${BACK_END_DIR} || exit_error "failed to create backend install directory"
${SUDO}  mkdir -p ${BACK_END_LOG_DIR} || exit_error "failed to create backend log directory"
${SUDO}  mkdir -p ${BACK_END_TMP_DIR} || exit_error "failed to create backend tmp  directory"
${SUDO}  mkdir -p ${BACK_END_BIN_DIR} || exit_error "failed to create backend bin directory"
${SUDO}  ln -s ${BACK_END_DIR} ${LINK_DIRECTORY}
if [[ $EUID -ne 0 ]]; then
    if [ -n "$USER" ]; then
        ${SUDO} chown -R $USER ${BACK_END_DIR} || exit_error "failed to assign directory to user $USER"
        ${SUDO} chown -R $USER ${LINK_DIRECTORY} || exit_error "failed to assign directory ${LINK_DIRECTORY} to user $USER"
    fi
fi

if [ ! -f ${SOURCE_PREP_EXE} ]; then
    exit_error "missing source prep script"
fi
if [ ! -f ${DUP_DETECT_EXE} ]; then
    exit_error "missing duplication detection application"
fi
if [ ! -f ${DATA_CLEAN_EXE} ]; then
    exit_error "missing data cleaning application"
fi

if [ ! -f ${EXPERS_EXE} ]; then
    exit_error "missing experiments exe"
fi


`ls ${DATA_PREP_EXE} > /dev/null 2>&1` || exit_error "missing data prep script"
cp -f ${SOURCE_PREP_EXE} ${BACK_END_BIN_DIR} || exit_error "failed to copy ${SOURCE_PREP_EXE} to ${BACK_END_BIN_DIR}"
cp -f ${CREATE_MODEL} ${BACK_END_BIN_DIR} || exit_error "failed to copy ${CREATE_MODEL} to ${BACK_END_BIN_DIR}"
cp -f ${DUP_DETECT_EXE} ${BACK_END_BIN_DIR} || exit_error "failed to copy ${DUP_DETECT_EXE} to ${BACK_END_BIN_DIR}"
cp -f ${DATA_CLEAN_EXE} ${BACK_END_BIN_DIR} || exit_error "failed to copy ${DATA_CLEAN_EXE} to ${BACK_END_BIN_DIR}"
cp -f ${DATA_PREP_EXE} ${BACK_END_BIN_DIR} || exit_error "failed to copy data prep exe to ${BACK_END_BIN_DIR}"
cp -f ${EXPERS_EXE} ${BACK_END_BIN_DIR} || exit_error "failed to copy experiments exe to ${BACK_END_BIN_DIR}"
cp -f ${MODEL_PRED} ${BACK_END_BIN_DIR} || exit_error "failed to copy model prediction exe to ${BACK_END_BIN_DIR}"
cp -f latest_version ${BACK_END_DIR}/version || exit_error "failed to save version number file"

env_pp=$(grep PYTHONPATH ~/.bashrc | grep -v ${BACK_END_BIN_DIR})
if [ -z "$env_pp" ]; then
    echo "export PYTHONPATH=$PYTHONPATH:${BACK_END_BIN_DIR}" >> ~/.bashrc
fi

echo "successfully copy binaries files"
echo "successfully finish to install backend application to ${BACK_END_DIR}"
exit 0
