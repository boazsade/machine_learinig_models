#!/usr/bin/env bash

VERSION_NUMBER=2.1.1
SPARK_INSTALL_VERSION=spark-${VERSION_NUMBER}-bin-hadoop2.7

BASE_DIRECTORY=/opt
INSTALL_DIRCTORY=binah
TARGET_DIRECTORY=${BASE_DIRECTORY}/${INSTALL_DIRCTORY}
BACK_END_THIRD_PATRIES=${TARGET_DIRECTORY}/third_parties
SPARK_DIR=${BACK_END_THIRD_PATRIES}/spark
NATIVE_LIBS_SPARK=${SPARK_DIR}/lib

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

function install_spark {
    echo "trying to install spark at ${SPARK_DIR}, checking for JAVA and other modeuls"
    # make sure that we have JAVA installed
    host_defined=$(grep -c `hostname` /etc/hosts)
    if [ "$host_defined" = "0" ]; then
        echo "missing host name from /etc/hosts - spark required that this would be set currectly"
        my_host=`hostname`
        exit_error "edit /etc/hosts and add ' $my_host ' with the right interface address (such as 127.0.0.1) to it"
    fi

    java -version >/dev/null 2>&1 || exit_error "you must install JAVA before you can contiune"
    python -c "import py4j" 2>&1 || exit_error "you must install py4j module (using pip install py4j) before you can continue"
    if [ ! -f ${SPARK_INSTALL_VERSION}.tgz ]; then
        exit_error "missing spark installation file"
    fi
    # extract spark installed files from tar and set env  
    tar -zxf ${SPARK_INSTALL_VERSION}.tgz > /dev/null 2>&1 || exit_error "failed to extract spark files"
    ${SUDO} mv ${SPARK_INSTALL_VERSION} ${SPARK_DIR} || exit_error "failed to move spark files to ${SPARK_DIR}"
    ${SUDO} mkdir -p ${NATIVE_LIBS_SPARK} > /dev/null 2>&1 || exit_error "failed to create directory ${NATIVE_LIBS_SPARK}"
    ${SUDO} cp -f libhadoop.so* ${NATIVE_LIBS_SPARK} || exit_error "failed to register native hadoop library"
    ${SUDO} cp -f libhdfs.so* ${NATIVE_LIBS_SPARK} || exit_error "failed to register native hadoop library"
    ${SUDO} chmod -R 777 ${NATIVE_LIBS_SPARK} > /dev/null 2>&1
    env_sphome=$(grep SPARK_HOME ~/.bashrc | grep -c ${SPARK_DIR})
    env_pp=$(grep PYTHONPATH ~/.bashrc | grep -v ${SPARK_DIR}/python)
    if [ "$env_sphome" = "0" ]; then
        echo "export SPARK_HOME=${SPARK_DIR}" >> ~/.bashrc
        echo "export PATH=$PATH:${SPARK_DIR}/bin" >> ~/.bashrc
    fi
    lib_env=$(grep HADOOP_COMMON_LIB_NATIVE_DIR ~/.bashrc | grep -c ${NATIVE_LIBS_SPARK})
    if [ "$lib_env" = "0" ]; then
        echo "HADOOP_COMMON_LIB_NATIVE_DIR=${NATIVE_LIBS_SPARK}" >> ~/.bashrc
        echo "HADOOP_OPTS=\"-Djava.library.path=${NATIVE_LIBS_SPARK}\"" >>  ~/.bashrc
        if [ -n ${LD_LIBRARY_PATH} ];then
            echo "LD_LIBRARY_PATH=${NATIVE_LIBS_SPARK}:${LD_LIBRARY_PATH}">> ~/.bashrc
        else
            echo "LD_LIBRARY_PATH=${NATIVE_LIBS_SPARK}" >> ~/.bashrc
        fi
    fi
    if [ -z "$env_pp" ]; then
        echo "export PYTHONPATH=${SPARK_DIR}/python" >> ~/.bashrc
    fi
    echo "successfully install spark at ${SPARK_DIR}"
}
echo -e "this would try to install spark on the location machine to ${BACK_END_THIRD_PATRIES}\nplease note that the installation would not run if this already installed"
if [ ! -d ${BACK_END_THIRD_PATRIES} ]; then
    ${SUDO}  mkdir -p ${BACK_END_THIRD_PATRIES} || exit_error "failed to create third parties directory"
    install_spark
fi
if [ ! -d ${SPARK_DIR}/bin ]; then
    install_spark
fi
echo "installation completed"

