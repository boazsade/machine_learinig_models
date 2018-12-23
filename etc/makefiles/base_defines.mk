project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
#include $(project_root)/etc/makefiles/third_party_defines.mk
include $(project_root)/etc/makefiles/compiler_setting.mk
ifeq ($(DEBUG),)
  ifeq ($(ALL_LOGS),)
  else
    CFLAGS += -DENABLE_DEBUG_LOGS_COMPILE_TIME 
  endif
    CFLAGS += -O3 -DNDEBUG
    SUB_DIR_NAME=release
else
    DEBUG_PATH=_debug
    SUB_DIR_NAME=debug
    CFLAGS += -g -DENABLE_DEBUG_LOGS_COMPILE_TIME
endif
# -fsanitize=address
ifeq ($(MY_ARCH),)
    UNAME_ARCH = $(shell uname -m)
    ifeq ($(UNAME_ARCH), i686)
       ARCH = -march=pentium4
    endif
    ifeq ($(UNAME_ARCH), x86_64)
      ARCH = -m64
      ARCH_PATH=$(UNAME_ARCH)
    else
      ARCH_PATH=$(UNAME_ARCH)
    endif
endif

LIB_NAME_PREFIX := binah_
LIB_PREFIX_NAME := lib$(LIB_NAME_PREFIX)
LIB_MOCK_SUFFIX := _mock
OBJDIR := .objs/$(SUB_DIR_NAME)
LIBDIR := $(project_root)/build/lib/$(SUB_DIR_NAME)
LIBMOCKDIR := $(project_root)/build/libmocks/$(SUB_DIR_NAME)
UNITTESTSDIR := $(project_root)/build/unittests/$(SUB_DIR_NAME)
APP_DIR := $(project_root)/build/bin/$(SUB_DIR_NAME)
TEST_DIR := $(project_root)/build/test/$(SUB_DIR_NAME)
POC_DIR := $(project_root)/build/pocs/$(SUB_DIR_NAME)
MOCKSOURCEDIR := $(project_root)/mocks
DEPDIR := .depends/$(SUB_DIR_NAME)
SOFILES_DIR := $(project_root)/build/sharedobjs/$(SUB_DIR_NAME)
CXXFLAGS ?= $(BOOST_INCLUDE_FLAG) $(ZLIB_INCLUDE_FLAG) $(BZIP2_INCLUDE_FLAG) $(OPEN_SSL_INCLUDE_FLAG)
LIBS_SOURCE_DIR ?= $(project_root)/src/libs/
APPS_SOURCE_DIR ?= $(project_root)/src/apps/
SOLIBS_SOURCE_DIR ?= $(project_root)/src/shared_libs/ 
TEST_SOURCE_DIR ?= $(project_root)/src/tests/
