
project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)

include $(project_root)/etc/makefiles/pre_req_install.mk
include $(project_root)/etc/makefiles/default_include.mk
include $(project_root)/etc/makefiles/base_defines.mk
include $(project_root)/etc/makefiles/source_list.mk

CFLAGS4CPP2 := $(filter-out -Wstrict-prototypes, $(CFLAGS))
CFLAGS4CPP3 := $(filter-out -Wmissing-prototypes, $(CFLAGS4CPP2))
CFLAGS4CPP4 := $(filter-out -Wold-style-definition, $(CFLAGS4CPP3))
CFLAGS4CPP := $(filter-out -Wnested-externs, $(CFLAGS4CPP4))

#$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = @$(CC) $(DEPFLAGS) $(CFLAGS) $(STDCFLAG) $(CPPFLAGS) $(INCLUDES) $(TARGET_ARCH) -c
COMPILE.cc = @$(CXX) $(DEPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(CFLAGS4CPP) $(INCLUDES) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(OBJDIR)/%.o : %.c
$(OBJDIR)/%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
	@echo "finish compiling $<"

$(OBJDIR)/%.o : %.cc
$(OBJDIR)/%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
	@echo "finish compiling $<"

$(OBJDIR)/%.o : %.cxx
$(OBJDIR)/%.o : %.cxx $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
	@echo "finish compiling $<"

$(OBJDIR)/%.o : %.cpp
$(OBJDIR)/%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
	@echo "finish compiling $<"

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

$(OBJDIR) : | $(DEPDIR)
	@mkdir -p $(OBJDIR)

$(DEPDIR) : 
	@mkdir -p $(DEPDIR)

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))


