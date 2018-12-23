# any definitions belong to the project of 3rd parties libraries should be added here
# please do not use direct versions but define them so that 
# we would have trecking for version for any library we are using
#project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)

#include $(project_root)/etc/makefiles/base_defines.mk
#include $(project_root)/etc/makefiles/compiler_setting.mk

HIREDIS_VERSION := 3.2.6
#OPEN_SSL_VERSION := 1.0.1
#PCAP_VERSION := 1.8.1
#BZIP2_VERSION := 1.0.6
#FILE_VERSION := 5.29
THIRD_PARTIES_DIR := $(shell $(project_root)/etc/makefiles/get_base_directory.sh version_3rd.h 10 $(project_root))

ifeq ($(THIRD_PARTIES_DIR),,)
  $(error "3rd parties directory is missing or cannot be found please clone from git  /opt/git.repo/3rd_parties/")
endif

BOOST_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "BOOST_VERSION")
LIBSVM_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "LIBSVM_VERSION")
ZLIB_VRESION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "ZLIB_VERSION")
LIGHGBM_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "LIGHGBM_VERSION")

THIRD_PARTIES_INCLUDE_DIR ?= $(THIRD_PARTIES_DIR)/include
BOOST_INCLUDE_BASE_DIRECTORY ?= $(THIRD_PARTIES_INCLUDE_DIR)/boost_$(BOOST_VERSION)
THRIRD_PARTIES_LIB_DIR ?= $(THIRD_PARTIES_DIR)/libs/$(ARCH_PATH)
BOOST_INCLUDE_FLAG := -isystem $(BOOST_INCLUDE_BASE_DIRECTORY)
THRIRD_PARTIES_LIB_C := $(THRIRD_PARTIES_LIB_DIR)/$(SUB_DIR_NAME)
THRIRD_PARTIES_LIB_CXX_BASE_PATH := $(THRIRD_PARTIES_LIB_C)/gcc_$(GCC_VERSION_FROM_SHELL)
BOOST_LIB_PATH := $(THRIRD_PARTIES_LIB_CXX_BASE_PATH)/boost/$(BOOST_VERSION)
REDIS_INCLUDE ?= $(THIRD_PARTIES_INCLUDE_DIR)/hiredis/redis-${HIREDIS_VERSION}
REDIS_LIB_PATH ?= ${THRIRD_PARTIES_LIB_C}/hiredis/hiredis-${HIREDIS_VERSION}
LIBSVM_PATH ?= $(THRIRD_PARTIES_LIB_CXX_BASE_PATH)/libsvm/libsvm-$(LIBSVM_VERSION)
LIGHTGBM_PATH ?= $(THRIRD_PARTIES_LIB_CXX_BASE_PATH)/LightGBM/LightGBM-$(LIGHGBM_VERSION)


ZLIB_INCLUDE ?= ${THIRD_PARTIES_INCLUDE_DIR}/zlib/zlib-${ZLIB_VRESION}
LIBSVM_INCLUDE ?= ${THIRD_PARTIES_INCLUDE_DIR}/libsvm/libsvm-${LIBSVM_VERSION}
LIGHTGBM_INCLUDE ?= ${THIRD_PARTIES_INCLUDE_DIR}/LightGBM/LightGBM-${LIBSVM_VERSION}
ZLIB_INCLUDE_FLAG ?= -isystem ${ZLIB_INCLUDE}
ZLIB_LIB_PATH ?= ${THRIRD_PARTIES_LIB_C}/zlib/zlib-${ZLIB_VRESION}
REDIS_INCLUDE_FLAG := -isystem $(REDIS_INCLUDE) 
LIBSVM_INCLUDE_FLAG ?= -isystem $(LIBSVM_INCLUDE)
LIGHTGBM_INCLUDE_FLAG ?= -isystem $(LIGHTGBM_INCLUDE)

