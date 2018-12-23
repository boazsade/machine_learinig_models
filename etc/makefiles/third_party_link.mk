project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/third_party_defines.mk
include $(project_root)/etc/makefiles/deep_leaning_defines.mk
include $(project_root)/etc/makefiles/opencv_link.mk
REDIS_NAME := hiredis
ZLIB_NAME := z
PCAP_NAME := pcap
FILE_NAME := magic
BZIP_NAME = bz2
OPENSSL_NAME := ssl
CRYPTO_NAME := crypto
OPENCV_NAME := opencv
LIBSVM_NAME := svm
LIGHTGBM_NAME := lightgbm
BOOST_LIBS_LIST := $(addprefix -l, $(BOOST_LIBS))
ifneq ($(filter $(OPENCV_NAME),$(APP_EXTRA_LIB)),)
      OPENCV_LIB_INC := $(addprefix -l, $(OPENCV_LIBS_LIST))
      APP_FRAMEWORK_LIBS += -L$(OPENCV_LIB_PATH) $(OPENCV_LIB_INC)
      APP_EXTRA_LIB := $(filter-out $(OPENCV_NAME), $(APP_EXTRA_LIB))
endif
ifdef BOOST_LIBS
    BOOST_LIBS_LIST_T := $(addprefix -l, $(BOOST_LIBS))
    BOOST_LIBS_LIST := -L$(BOOST_LIB_PATH) $(BOOST_LIBS_LIST_T)
endif

APP_FRAMEWORK_LIBS += $(BOOST_LIBS_LIST) $(SNIFFER_LIBS_LIST)

ifneq ($(filter $(ZLIB_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(ZLIB_LIB_PATH) -l$(ZLIB_NAME)
    APP_EXTRA_LIB := $(filter-out $(ZLIB_NAME), $(APP_EXTRA_LIB)) 
endif

ifneq ($(filter $(PCAP_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(PCAP_LIB_PATH) -l$(PCAP_NAME)
    APP_EXTRA_LIB := $(filter-out $(PCAP_NAME), $(APP_EXTRA_LIB)) 
endif

ifneq ($(filter $(BZIP_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(BZIP2_LIB_PATH) -l$(BZIP_NAME)
    APP_EXTRA_LIB := $(filter-out $(BZIP_NAME), $(APP_EXTRA_LIB)) 
endif

ifneq ($(filter $(FILE_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(FILE_LIB_PATH) -l$(FILE_NAME)
    APP_EXTRA_LIB := $(filter-out $(FILE_NAME), $(APP_EXTRA_LIB)) 
endif

ifneq ($(filter $(OPENSSL_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(OPEN_SSL_LIB_PATH) -l$(OPENSSL_NAME) -l$(CRYPTO_NAME)
    APP_EXTRA_LIB := $(filter-out $(OPENSSL_NAME), $(APP_EXTRA_LIB)) 
    APP_EXTRA_LIB := $(filter-out $(CRYPTO_NAME), $(APP_EXTRA_LIB)) 
endif

ifneq ($(filter $(REDIS_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(REDIS_LIB_PATH) -l$(REDIS_NAME)
    APP_EXTRA_LIB := $(filter-out $(REDIS_NAME), $(APP_EXTRA_LIB)) 
endif
ifneq ($(filter $(LIBSVM_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(LIBSVM_PATH) -l$(LIBSVM_NAME)
    APP_EXTRA_LIB := $(filter-out $(LIBSVM_NAME), $(APP_EXTRA_LIB)) 
endif
ifneq ($(filter $(LIGHTGBM_NAME),$(APP_EXTRA_LIB)),)
    APP_FRAMEWORK_LIBS += -L$(LIGHTGBM_PATH) -l$(LIGHTGBM_NAME)
    APP_EXTRA_LIB := $(filter-out $(LIBSVM_NAME), $(APP_EXTRA_LIB)) 
endif

SYSTEM_LIBS := $(addprefix -l, $(APP_EXTRA_LIB))
