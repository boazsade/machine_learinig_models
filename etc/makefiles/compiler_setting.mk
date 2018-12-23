project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/third_party_defines.mk
# take the compiler from the the tool directory in the 3rdparties repository
COMPILER_VERSION:=6.1.0
COMPILER_BASE_DIR:=$(THIRD_PARTIES_DIR)/tools/gcc-$(COMPILER_VERSION)
COMPILER_BIN_DIR:=$(COMPILER_BASE_DIR)/bin
COMPILER_LIB_DIR:=$(COMPILER_BASE_DIR)/lib
COMPILER_LIB64_DIR:=$(COMPILER_BASE_DIR)/lib64
COMPILER_INCLUDE_DIR:=$(COMPILER_BASE_DIR)/include
CC:=$(COMPILER_BIN_DIR)/gcc
CXX:=$(COMPILER_BIN_DIR)/g++ 
AR:=$(COMPILER_BIN_DIR)/gcc-ar
RANLIB := $(COMPILER_BIN_DIR)/gcc-ranlib
GCC_VERSION_FROM_SHELL := $(shell $(CC) -dumpversion | cut -d '.' -f 1-2 | sed 's/\./_/')

