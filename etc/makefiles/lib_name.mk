include $(project_root)/etc/makefiles/base_defines.mk
include $(project_root)/etc/makefiles/getlib_name.mk


ifeq ($(origin LIB_NAME), undefined)
else
LIB_NAME_FULL1 := $(addprefix $(LIB_PREFIX_NAME), $(LIB_NAME))
LIB_NAME_FULL := $(LIB_NAME_FULL1).a
LIB ?= $(addprefix $(LIBDIR)/, $(LIB_NAME_FULL))
endif


ifeq ($(origin LIB_MOCK), undefined)
else
LIB_MOCK_FULL1 := $(addprefix $(LIB_PREFIX_NAME), $(LIB_MOCK))
LIBMOCK_NAME := $(LIB_MOCK_FULL1)$(LIB_MOCK_SUFFIX).a
LIBMOCK := $(addprefix $(LIBMOCKDIR)/, $(LIBMOCK_NAME))
endif

