project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )

BOOST_LIBS += boost_log boost_log_setup  \
	      boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system
	      
LIBS_LIST := dataclean_mode_use rows_drops \
    sources_handling json_parser \
    system utils logging
APP_EXTRA_LIB += opencv z dl pthread
PARENT_DIR_1 := $(shell pwd)
PARENT_DIR := $(shell dirname $(PARENT_DIR_1))
APP_BASE_NAME := $(shell basename $(PARENT_DIR))
CPPFLAGS += -DSUPPORT_FOR_MODEL_USE
include $(project_root)/etc/makefiles/app_builder.mk
