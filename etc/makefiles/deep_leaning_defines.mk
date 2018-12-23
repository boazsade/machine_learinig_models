
project_root ?= $(shell while [ ! -f build.sh ]; do cd ..; done ; pwd)
include $(project_root)/etc/makefiles/third_party_defines.mk

GFLAG_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "GFLAG_VERSION")
HDF5_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "HDF5_VERSION")
LEVELDB_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "LEVELDB_VERSION")
OPENCV_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "OPENCV_VERSION")
PROTOBUF_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "PROTOBUF_VERSION")
LMDB_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "LMDB_VERSION")
CAFFE_VERSION := $(shell $(THIRD_PARTIES_DIR)/read_lib_version.sh $(THIRD_PARTIES_DIR) "CAFFE_VERSION")

DL_LIST_DIRS := cflags/cflags-$(CFLAG_VERSION) hdf5/hdf5-$(HDF5_VERSION) leveldb/leveldb-$(LEVELDB_VERSION) opencv/opencv-$(OPENCV_VERSION) opencv/opencv-$(OPENCV_VERSION)/opencv  protobuf/protobuf-$(PROTOBUF_VERSION) glog

DL_INCLUDE_PATHS := $(addprefix $(THIRD_PARTIES_INCLUDE_DIR)/, $(DL_LIST_DIRS))
DL_INCLUDE_PATHS += lmdb/lmdb-$(LMDB_VERSION) 

INCLUDE_DL_OP := $(addprefix -isystem , $(DL_INCLUDE_PATHS))
DL_LIBS_PATHS := $(addprefix $(THRIRD_PARTIES_LIB_CXX_BASE_PATH), $(DL_LIST_DIRS))
DL_LIBS_PATHS += $(THRIRD_PARTIES_LIB_C)/lmdb/lmdb-$(LMDB_VERSION)
OPENCV_LIB_PATH := $(THRIRD_PARTIES_LIB_CXX_BASE_PATH)/opencv/opencv-$(OPENCV_VERSION)

