project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_thread boost_filesystem \
	      boost_system

APP_EXTRA_LIB += opencv svm z dl pthread
LIBS_LIST := transform_action scores_evaluation \
    classifier_models_actions \
    regression_models_actions \
    model_data experiments \
    ml_models sources_handling \
    system json_parser utils logging

INCLUDE_DIRS += include 
INTERNAL_SRC := $(APP_NAME)
include $(project_root)/etc/makefiles/app_builder.mk
#.PHONY: app

#app:
#	@echo "hello"
