project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/anyinclude_dir.mk
include $(project_root)/etc/makefiles/lib_name.mk
include $(project_root)/etc/makefiles/static_libs.mk

