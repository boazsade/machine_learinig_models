project_root=$(shell while [ ! -f build.sh ]; do cd ..; done ; pwd )

BOOST_LIBS += boost_log boost_log_setup  \
	      boost_unit_test_framework boost_test_exec_monitor \
	      boost_thread boost_filesystem \
	      boost_system
	      
LIBS_LIST := ml_models utils logging
APP_EXTRA_LIB += opencv z dl pthread svm
# -DPRINT_MODEL_RESULTS
CXXFLAGS += $(INCLUDE_DL_OP) $(BOOST_INCLUDE_FLAG) $(LIBSVM_INCLUDE_FLAG)
include $(project_root)/etc/makefiles/app_builder.mk
