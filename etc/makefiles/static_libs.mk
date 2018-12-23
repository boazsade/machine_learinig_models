project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)

include $(project_root)/etc/makefiles/base_defines.mk
include $(project_root)/etc/makefiles/build_base.mk
include $(project_root)/etc/makefiles/deep_leaning_defines.mk

.PHONY: all $(LIB)
.PHONY: build_lib $(LIB)
.PHONY: clean

all: $(LIB)
build_lib: $(LIB)
	@echo "building library $(LIB) is done"

define create_static_lib	
	@$(AR) rc $(LIB) $(OBJS)
	@$(RANLIB) $(LIB)
endef

define create_static_lib_mock
	@$(AR) rc $(LIBMOCK_NAME) $(OBJS)
	@$(RANLIB) $(LIBMOCK_NAME)
	@mv $(LIBMOCK_NAME) $(LIBMOCKDIR)
endef

$(OBJS) : | $(OBJDIR) 

#@echo "objs list is $(OBJS) objs list $(OBJLIST) SRCS = $(SRCS) "
$(LIB): $(OBJS) | $(LIBDIR)
	$(create_static_lib)
	
$(LIBMOCK): | $(OBJDIR)
	$(create_static_lib_mock)

$(LIBDIR): 
	@mkdir -p $(LIBDIR)
	
clean:
	@rm -rf $(DEPDIR) $(OBJDIR) $(LIB)
	@echo "finish cleaning $(LIB)"
