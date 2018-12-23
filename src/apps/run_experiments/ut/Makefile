project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )

ifeq ($(DUMP_ROW),1)
  CPPFLAGS += -DDUMP_ROW_MODEL_OUTPUT
endif

BOOST_LIBS += boost_log boost_log_setup  \
	      boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system
	      
LIBS_LIST := transform_action scores_evaluation\
    classifier_models_actions \
    regression_models_actions \
    model_data experiments ml_models sources_handling \
    system json_parser utils logging

APP_EXTRA_LIB += opencv z dl svm pthread
include $(project_root)/etc/makefiles/parent_dir_include.mk
include $(project_root)/etc/makefiles/app_builder.mk
