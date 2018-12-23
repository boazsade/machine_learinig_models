project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)

include $(project_root)/etc/makefiles/app_builder.mk

APPS_DIRECTORY := $(project_root)/src/apps
LIBS_DIRECTORY := $(project_root)/src/libs
VERSION_NUMBER:= $(shell $(project_root)/get_version.sh)
TARGET_REPO := $(project_root)/installs
TARGET_DIR := $(TARGET_REPO)
TARGET_FILE_BASE_NAME := backend_binaries
TARGET_VERSION_FILE := $(TARGET_DIR)/$(TARGET_FILE_BASE_NAME).tar.gz
APPS_DIRS_TMP :=  $(dir $(wildcard $(APPS_DIRECTORY)/*/Makefile))
APPS_DIRS:= $(dir $(APPS_DIRS_TMP))
LIBS_DIRS := $(dir $(wildcard $(LIBS_DIRECTORY)/*/ut/))
APP_UT_DIRS := $(dir $(wildcard $(APPS_DIRECTORY)/*/ut/))
LIBS_UT_DIRS := $(LIBS_DIRS)
#APPS_NAMES := $(wildcard $(APPS_DIRECTORY)/*)
#APPS_DIRS := $(addprefix $(APPS_DIRECTORY)/, $(APPS_NAMES)) 
UT_DIRS := $(LIBS_UT_DIRS) $(APP_UT_DIRS)


.PHONY: version
.PHONY: apps_build $(APPS_DIRS)
.PHONY: apps_uts $(UT_DIRS)
#.PHONY: commit
#.PHONY: version_commit
.PHONY: all $(UT_DIRS) $(APPS_DIRS)
.PHONY: install 
.PHONY: clean_all

#$(error "THIRD_PARTIES_DIR = $(THIRD_PARTIES_DIR)")
#$(error "apps dirs = $(APPS_DIRS)")

apps_build: $(APPS_DIRS)

$(THIRD_PARTIES_DIR):
	@echo "$(THIRD_PARTIES_DIR) missing!"
	exit 1

$(TARGET_DIR):
	@echo "creating directory $(TARGET_DIR)"
	@mkdir -p $(TARGET_DIR)	

$(UT_DIRS):
	@echo "running unit test $@"
	@$(MAKE) -C $@ ut

$(APPS_DIRS):
	@echo "building $@"
	@$(MAKE) -C $@ app

install:
	@echo "generating version $(VERSION_NUMBER) at $(TARGET_DIR)"
	@rm -f ${TARGET_DIR}/$(TARGET_VERSION_FILE)
	@$(project_root)/make_version.sh $(VERSION_NUMBER) $(TARGET_REPO) $(project_root) || exit 1
	@echo "successfully generated installation file $(TARGET_VERSION_FILE)"
#
$(TARGET_VERSION_FILE): apps_uts apps_build install $(THIRD_PARTIES_DIR) | $(TARGET_DIR) 
	@echo "finish build and unit testing"

apps_uts: $(UT_DIRS)

all: $(UT_DIRS) $(APPS_DIRS)

version: $(TARGET_VERSION_FILE)

clean_all:
	@rm -rf $(project_root)/build
	@find $(project_root) -name .objs | xargs rm -rf
	@find $(project_root) -name .depends | xargs rm -rf
