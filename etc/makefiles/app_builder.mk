project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
include $(project_root)/etc/makefiles/build_app.mk 


.PHONY: app $(APP_NAME)
.PHONY: clean_app
