# we have directories in which the sources are in src dir
# this would not help with getting lib name from it
# so we would like to make sure that we are not trying
# to give a "generic" name
LIB_NAME_FROM_PATH := $(shell basename `pwd`)
ifeq ($(LIB_NAME_FROM_PATH),src)
    LIB_LOCATION_PATH := $(shell cd .. ; pwd)
    LIB_NAME ?= $(notdir $(LIB_LOCATION_PATH))
else
    LIB_NAME ?= $(LIB_NAME_FROM_PATH)
endif
