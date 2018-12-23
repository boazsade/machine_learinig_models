project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/default_app_name.mk
BOOST_LIBS += boost_log boost_log_setup  \
	      boost_thread boost_filesystem \
	      boost_system

APP_EXTRA_LIB += opencv svm z dl pthread
LIBS_LIST := model_prediction sources_handling \
    regression_models_actions \
    classifier_models_actions ml_models \
    system json_parser utils logging
#CPPFLAGS += -DSUPPORT_FOR_MODEL_GENERATION
INTERNAL_SRC := $(APP_NAME)
include $(project_root)/etc/makefiles/app_builder.mk
