project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_thread boost_filesystem \
	      boost_regex boost_system \
	      boost_program_options
APP_EXTRA_LIB += pthread
LIBS_LIST := csv_parser json_parser logging
INTERNAL_SRC := $(APP_NAME)
include $(project_root)/etc/makefiles/app_builder.mk
