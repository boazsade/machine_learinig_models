PARENT_DIR_1 := $(shell pwd)
PARENT_DIR := $(shell dirname $(PARENT_DIR_1))
APP_BASE_NAME := $(shell basename $(PARENT_DIR))
INCLUDE_DIRS += $(PARENT_DIR)/include
INTERNAL_SRC := $(APP_BASE_NAME)
