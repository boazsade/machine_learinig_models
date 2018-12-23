
#glibc_header := $(shell test   -f /usr/include/features.h; echo $$? )
#glibc_devel := 0
#$(shell test -f /usr/include/gnu/stubs-64.h; echo $$? )
#ifneq ($(glibc_header),0)
#   $(error "you must install missing RPMS by running install_rpms.sh from 3rd_parties/tools/centos_7/rpms")
#endif

#ifneq ($(glibc_devel),0)
#    $(error "glibc_devel = $(glibc_devel) : you must install missing RPMS by running install_rpms.sh from 3rd_parties/tools/centos_7/rpms")
#endif
pre_required_install := $(shell $(project_root)/etc/makefiles/locate_preconditions.sh)
ifneq ($(pre_required_install),0)
    $("error "you need to install glibc header files to successfully build")
endif

