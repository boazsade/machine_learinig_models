project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_thread boost_filesystem \
	      boost_system

APP_EXTRA_LIB += opencv z dl pthread
LIBS_LIST := experiments classfication_algos json_parser utils logging
#INCLUDE_DIRS += include 
INTERNAL_SRC := $(TEST_NAME)
include $(project_root)/etc/makefiles/app_builder.mk
#.PHONY: app

#app:
#	@echo "hello"
