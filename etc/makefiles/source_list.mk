
project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/base_defines.mk
include $(project_root)/etc/makefiles/default_sources.mk


.SUFFIXES: .c .cc .cpp
INCLUDES := $(addprefix  -I, $(INCLUDE_DIRS))
SRCFILES_C := $(filter %.c, $(SRCS))
SRCFILES_CPP := $(filter %.cpp, $(SRCS))
SRCFILES_CC := $(filter %.cc, $(SRCS))
OBJCLIST := $(SRCFILES_C:.c=.o)
OBJLISTCPP := $(SRCFILES_CPP:.cpp=.o)
OBJLISTCC := $(SRCFILES_CC:.cc=.o)
OBJLIST := $(OBJCLIST) $(OBJLISTCC) $(OBJLISTCPP)
#$(error "SRCS = $(SRCS), SRCFILES_C = $(SRCFILES_C), SRCFILES_CPP = $(SRCFILES_CPP), SRCFILES_CC = $(SRCFILES_CC), OBJCLIST = $(OBJCLIST), OBJLISTCPP = $(OBJLISTCPP), OBJLISTCC = $(OBJLISTCC), OBJLIST = $(OBJLIST)")
OBJS := $(addprefix $(OBJDIR)/, $(OBJLIST))

