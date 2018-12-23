project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
BOOST_LIBS += boost_unit_test_framework \
	      boost_test_exec_monitor \
	       boost_log boost_log_setup \
	      boost_thread boost_filesystem \
	      boost_system

LIBS_LIST := model_prediction sources_handling \
    logging system json_parser utils
APP_EXTRA_LIB += z dl pthread
include $(project_root)/etc/makefiles/app_builder.mk
