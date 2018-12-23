project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
# build a name that the exe is depends on that is based on the 
# library file - that is the location of the file as well as the file prefix name and the file sufix name
LIBS_NAMES_T := $(addprefix $(LIB_PREFIX_NAME), $(LIBS_LIST))
LIBS_NAMES_T2 := $(addprefix $(LIBDIR)/, $(LIBS_NAMES_T))
DEPENDS_LIBS := $(addsuffix .a, $(LIBS_NAMES_T2))
# shared libs
SOLIBS_NAMES_T := $(addprefix $(LIB_PREFIX_NAME), $(SHARED_LIBS_LIST))
SOLIBS_NAMES_T2 := $(addprefix $(SOFILES_DIR)/, $(SOLIBS_NAMES_T))
DEPENDS_SOLIBS := $(addsuffix .so, $(SOLIBS_NAMES_T2))

