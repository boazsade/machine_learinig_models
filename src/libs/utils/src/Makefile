# find the location of the root for all files/directories in the project
project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )

# and the make file that would run it all
include $(project_root)/etc/makefiles/deep_leaning_defines.mk
CXXFLAGS += $(INCLUDE_DL_OP) $(BOOST_INCLUDE_FLAG)
include $(project_root)/etc/makefiles/default_lib_build.mk

