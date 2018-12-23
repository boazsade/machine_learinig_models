#this make file would build the sources
#given the list of sources that we are 
#inputing to it
project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/pre_req_install.mk
include $(project_root)/etc/makefiles/lib_name.mk
include $(project_root)/etc/makefiles/build_base.mk

DEPLIST_BY_OBJ := $(OBJLIST:.o=.d)
DEPLIST_BY_OBJ1 := $(addprefix /, $(DEPLIST_BY_OBJ))
DEPLIST_FILES := $(addprefix $(DEPDIR), $(DEPLIST_BY_OBJ1))
#$(error "dependencies files = $(DEPLIST_FILES)")
-include $(DEPLIST_FILES)
#$(error "OBJLIST = $(OBJLIST), DEPLIST_FILES = $(DEPLIST_FILES)")
CFLAGS4CPP2 := $(filter-out -Wstrict-prototypes, $(CFLAGS))
CFLAGS4CPP3 := $(filter-out -Wmissing-prototypes, $(CFLAGS4CPP2))
CFLAGS4CPP4 := $(filter-out -Wold-style-definition, $(CFLAGS4CPP3))
CFLAGS4CPP := $(filter-out -Wnested-externs, $(CFLAGS4CPP4))

define create_static_lib	
	@ar rc $(LIB) $(OBJS)
	@ranlib $(LIB)
endef

define create_static_lib_mock
	@ar rc $(LIBMOCK_NAME) $(OBJS)
	@ranlib $(LIBMOCK_NAME)
	@mv $(LIBMOCK_NAME) $(LIBMOCKDIR)
endef
$(LIB): dep $(OBJDIR) 
	$(create_static_lib)
	@echo "finish building $(notdir $(LIB))"
	
$(LIBMOCK): dep $(OBJDIR)
	$(create_static_lib_mock)
	

define create-dependencies	
	@mv $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	    sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp
endef

define create-cpp-objcts
	@echo "compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS4CPP) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@if [ ! -f ${OBJDIR}/$(notdir $@) ]; then cp $@ ${OBJDIR}  ;fi
	@mkdir -p ${DEPDIR}/$(dir $*)
	@$(CXX) -MM $(CFLAGS4CPP) $(CXXFLAGS) $(INCLUDES) $< > $(DEPDIR)/$*.d
	$(create-dependencies)
endef

define create-object
	@echo "compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@if [ ! -f ${OBJDIR}/$(notdir $@) ]; then cp $@ ${OBJDIR}  ;fi
	@mkdir -p ${DEPDIR}/$(dir $*)
	@$(CC) -MM $(CFLAGS) $(INCLUDES) $< > $(DEPDIR)/$*.d
	$(create-dependencies)
endef

$(OBJDIR)/%.o : %.cc
	$(create-cpp-objcts)


$(OBJDIR)/%.o : %.c
	$(create-object)	

$(OBJDIR)/%.o : %.cpp
	$(create-cpp-objcts)
	

$(DEPS) : | $(DEPDIR)


$(DEPDIR): 
	@mkdir -p $(DEPDIR)

depend: dep

dep: $(DEPS)
