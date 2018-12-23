project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )

BOOST_LIBS += boost_log boost_log_setup  \
	      boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system
	      
LIBS_LIST := json_parser csv_parser utils logging
APP_EXTRA_LIB += opencv z dl pthread
PARENT_DIR_1 := $(shell pwd)
PARENT_DIR := $(shell dirname $(PARENT_DIR_1))
APP_BASE_NAME := $(shell basename $(PARENT_DIR))
INCLUDE_DIRS += $(PARENT_DIR)/include
INTERNAL_SRC := $(APP_BASE_NAME)

include $(project_root)/etc/makefiles/app_builder.mk
