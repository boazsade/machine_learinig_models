project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
BOOST_LIBS += boost_regex boost_system boost_program_options
APP_EXTRA_LIB += opencv z
#LIBS_LIST := utils
CXXFLAGS += $(INCLUDE_DL_OP) $(BOOST_INCLUDE_FLAG)
include $(project_root)/etc/makefiles/app_builder.mk
