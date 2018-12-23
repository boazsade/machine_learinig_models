project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/compiler_setting.mk
DEFAULT_INCLUDE_DIR := $(project_root)/src/include
# make all warning as an errors
CFLAGS += -Werror -Wextra -Wunused -Wno-cpp -Wdouble-promotion
CFLAGS += -I$(DEFAULT_INCLUDE_DIR) -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS
# use the most up to date compiler langauge support
ifeq ($(GCC_VERSION_FROM_SHELL),4_8)
  STDCFLAG += -std=c90
  CXXFLAGS += -std=c++11
else
  STDCFLAG += -std=gnu11
  CXXFLAGS += -std=gnu++14
endif

