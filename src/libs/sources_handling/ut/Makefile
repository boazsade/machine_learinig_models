project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system

APP_EXTRA_LIB += pthread
LIBS_LIST := sources_handling ml_models system utils logging
include $(project_root)/etc/makefiles/app_builder.mk
