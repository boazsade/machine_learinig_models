project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/pre_req_install.mk
include $(project_root)/etc/makefiles/base_defines.mk
include $(project_root)/etc/makefiles/compiler_setting.mk
include $(project_root)/etc/makefiles/third_party_defines.mk
include $(project_root)/etc/makefiles/static_libs.mk
include $(project_root)/etc/makefiles/third_party_link.mk
include $(project_root)/etc/makefiles/libs_listing.mk
include $(project_root)/etc/makefiles/deep_leaning_defines.mk

# join it to the list of libraries that we are using

LIBS_FOR_BUILD := $(addprefix $(LIBS_SOURCE_DIR), $(LIBS_LIST))
ifeq ($(origin INTERNAL_SRC), undefined) 
else
 ifdef $(TESTDISABLE)
  APP_INTERNAL_LIBS_1 := $(addprefix $(APPS_SOURCE_DIR), $(INTERNAL_SRC))
 endif
 ifdef $(DOTEST)
  APP_INTERNAL_LIBS_1 := $(addprefix $(TEST_SOURCE_DIR), $(INTERNAL_SRC))
 endif
 ifdef $(DOUNITTEST)
  APP_INTERNAL_LIBS_1 := $(addprefix $(APPS_SOURCE_DIR), $(INTERNAL_SRC))
 endif
 APP_INTERNAL_LIBS := $(addsuffix /src, $(APP_INTERNAL_LIBS_1))
#$(error "INTERNAL_SRC = $(INTERNAL_SRC), APPS_SOURCE_DIR = $(APPS_SOURCE_DIR), APP_INTERNAL_LIBS_1 = $(APP_INTERNAL_LIBS_1), APP_INTERNAL_LIBS = $(APP_INTERNAL_LIBS)")
endif
SOS_FOR_BUILD := $(addprefix $(SOLIBS_SOURCE_DIR), $(SHARED_LIBS_LIST))
LIBS := $(addprefix -l$(LIB_NAME_PREFIX), $(INTERNAL_SRC))
LIBS += $(addprefix -l$(LIB_NAME_PREFIX), $(LIBS_LIST))
SOLIBS := $(addprefix  -l$(LIB_NAME_PREFIX), $(SHARED_LIBS_LIST))
# note that right now (until we would have full install) we would link stdc++ and gcc statically as they are not 
# the one that come with the OS but the one that come with the compiler that is in our source control
LDFLAGS += -L$(LIBDIR) $(LIBS) -L$(SOFILES_DIR) $(SOLIBS) -static-libstdc++ -static-libgcc

.PHONY: app
.PHONY: test
.PHONY: ut
.PHONY: poc
.PHONY: clean_app
.PHONY: clean_test
.PHONY: clean_ut
.PHONY: clean_poc
.PHONY: clean_sublibs
.PHONY: sub_libs $(LIBS_FOR_BUILD) $(APP_INTERNAL_LIBS)

ifneq ($(UNITTEST_NAME),)
endif

#$(UNITTESTSDIR)/libasan.so:
#	@ln -s $(COMPILER_LIB64_DIR)/libasan.so.3.0.0 $(UNITTESTSDIR)
#	@ln -s $(UNITTESTSDIR)/libasan.so.3.0.0 $(UNITTESTSDIR)/libasan.so

define build_app
	@echo "start building application $(APP_NAME)"
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(APP_FRAMEWORK_LIBS) $(LDLIBS) $(SYSTEM_LIBS)
endef 
#$(COMPILER_LIB64_DIR)"
define run_unittest
	@echo "start running unit test $(UNITTEST_NAME) "
	@$(project_root)/etc/makefiles/run_unittest.sh $(UNITTEST_NAME) $(UNITTESTSDIR) $(COMPILER_LIB64_DIR) || (echo -e "!!!!!!!!!!!!!!!!!!!!\n unit test $(UNITTEST_NAME) failed"; echo "!!!!!!!!!!!!!!!!!!!!";mv -f $(UNITTESTSDIR)/$(UNITTEST_NAME) $(UNITTESTSDIR)/$(UNITTEST_NAME).fail; exit 1) 
endef
#@$(UNITTESTSDIR)/$(UNITTEST_NAME) ||  (echo -e "!!!!!!!!!!!!!!!!!!!!\n unit test $(UNITTEST_NAME) failed"; echo "!!!!!!!!!!!!!!!!!!!!";)

app: $(APP_NAME)
test: $(TEST_NAME)
ut: $(UNITTEST_NAME)

poc: $(POC_NAME)

clean_local:
	@rm -rf $(OBJDIR) $(DEPDIR)

clean_app: clean_sublibs clean_local
	@rm -rf $(APP_DIR)/$(APP_NAME) $(DEPDIR) $(OBJDIR)
	@echo "finish cleaning application $(APP_NAME)"

clean_test: clean_sublibs clean_local
	@rm -rf $(TEST_DIR)/$(TEST_NAME) $(DEPDIR) $(OBJDIR)
	@echo "finish cleaning application $(TEST_NAME)"

clean_ut: clean_sublibs clean_local
	@rm -rf $(UNITTESTSDIR)/$(UNITTEST_NAME) $(DEPDIR) $(OBJDIR)
	@echo "finish cleaning application $(UNITTESTSDIR)"


clean_poc: clean_sublibs clean_local
	@rm -rf $(TEST_DIR)/$(POC_NAME) 
	@echo "finish cleaning application $(POC_NAME)"


clean_sublibs:
	@for sub_lib in $(LIBS_FOR_BUILD); do make --no-print-directory -C $$sub_lib clean; done
	@for app_lib in $(APP_INTERNAL_LIBS); do make --no-print-directory -C $$app_lib clean; done
	@echo "finish cleaning sub libs"

$(LIBS_FOR_BUILD):
	@$(MAKE) --no-print-directory -C $@ build_lib


$(APP_INTERNAL_LIBS):
	@$(MAKE) --no-print-directory -C $@ build_lib


sub_libs: $(LIBS_FOR_BUILD) $(APP_INTERNAL_LIBS)
#	@echo "finish building $(LIBS_FOR_BUILD)"

$(APP_DIR):
	@mkdir -p $(APP_DIR)

$(TEST_DIR):
	@mkdir -p $(TEST_DIR)

$(UNITTESTSDIR):
	@mkdir -p $(UNITTESTSDIR)

$(POC_DIR):
	@mkdir -p $(POC_DIR)

$(SOFILES_DIR):
	@$(MAKE) --no-print-directory -C $@ build_so

ifdef $(TESTDISABLE)
$(APP_DIR)/$(APP_NAME): $(OBJS) sub_libs | $(APP_DIR)
	$(build_app)
#	@echo "finish building pre-required libs and objects for $@"

$(APP_NAME) : $(SOS_FOR_BUILD)  $(APP_DIR)/$(APP_NAME)
	@echo "finish building $(APP_NAME)"

endif

ifdef $(DOTEST)
$(TEST_DIR)/$(TEST_NAME) : $(OBJS) sub_libs | $(TEST_DIR) 
	$(build_app)
#	@echo "finish building pre-required libs and objects for $@"

$(TEST_NAME) : $(SOS_FOR_BUILD)  $(TEST_DIR)/$(TEST_NAME)
	@echo "finish building $(TEST_NAME)"

endif

ifdef $(BUILD_POC)
$(POC_DIR)/$(POC_NAME) : $(OBJS) sub_libs | $(POC_DIR)
	$(build_app)
	@echo "finish buildling $(POC_NAME)"

$(POC_NAME): $(SOS_FOR_BUILD) $(POC_DIR)/$(POC_NAME)
	@echo "finish building $(POC_NAME)"
endif

ifdef $(DOUNITTEST)
$(UNITTESTSDIR)/$(UNITTEST_NAME) : $(OBJS) sub_libs $(MOCKS_LIST) | $(UNITTESTSDIR) 
	$(build_app)
	@echo "unit test $(UNITTEST_NAME) successfully build"

#| $(UNITTESTSDIR)/libasan.so
$(UNITTEST_NAME) : $(SOS_FOR_BUILD)  $(UNITTESTSDIR)/$(UNITTEST_NAME) 
	$(run_unittest)
	@echo "$(UNITTEST_NAME) finished successfully"

endif
