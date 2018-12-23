project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
BOOST_LIBS += boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system

#APP_EXTRA_LIB += magic
LIBS_LIST := sources_handling utils
include $(project_root)/etc/makefiles/app_builder.mk
