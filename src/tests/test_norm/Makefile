project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
BOOST_LIBS += boost_regex boost_system boost_program_options
#APP_EXTRA_LIB += magic
LIBS_LIST := utils
include $(project_root)/etc/makefiles/app_builder.mk
