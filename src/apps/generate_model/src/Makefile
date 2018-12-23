# find the location of the root for all files/directories in the project
project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )
ifeq ($(DUMP_ROW),1)
  CPPFLAGS += -DDUMP_ROW_MODEL_OUTPUT
endif

# and the make file that would run it all
include $(project_root)/etc/makefiles/default_lib_build.mk
