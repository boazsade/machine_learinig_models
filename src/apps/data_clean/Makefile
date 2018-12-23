project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_thread boost_filesystem \
	      boost_system

APP_EXTRA_LIB += opencv z dl pthread
LIBS_LIST := dataclean_mode_gen rows_drops \
    json_parser sources_handling \
    system utils logging
CPPFLAGS += -DSUPPORT_FOR_MODEL_GENERATION
include $(project_root)/etc/makefiles/app_builder.mk
