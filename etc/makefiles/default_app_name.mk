#create a name for the application
MY_PATH := $(shell pwd)
IS_TEST:=$(findstring tests, $(MY_PATH))
ISUNITTEST := $(findstring ut, $(MY_PATH))
ISPOC_BUILD := $(findstring pocs, $(MY_PATH))
ifeq ($(IS_TEST),)
    ifneq ($(ISUNITTEST),)
      IS_UNITTEST:=true
      DOUNITTEST = IS_UNITTEST
    else
      ifneq ($(ISPOC_BUILD),)
        ISPOC:=true
        BUILD_POC = ISPOC	
      else
        NOT_TEST:=true
        TESTDISABLE = NOT_TEST
      endif
  endif
else
    TEST_ENABLE:=true
    DOTEST = TEST_ENABLE
endif
ifdef $(TESTDISABLE) 
    #APP_NAME_FROM_PATH := $(shell basename `pwd`)
    APP_NAME_FROM_PATH :=  $(shell basename $(MY_PATH))
    ifeq ($(APP_NAME_FROM_PATH),src) 
       APP_LOCALTION := $(shell cd .. ; pwd)
       APP_NAME ?= $(notdir $(APP_LOCALTION))
    else
       APP_NAME ?= $(APP_NAME_FROM_PATH)
    endif 
else
    ifdef $(DOTEST)
      TEST_NAME_FROM_PATH:= $(shell basename $(MY_PATH))
      TEST_NAME ?= $(TEST_NAME_FROM_PATH)
    endif
    ifdef $(DOUNITTEST)
      UTTEST_NAME_FROM_PATH := $(notdir $(patsubst %/,%,$(dir $(MY_PATH))))
      #$(shell basename `pwd`)
      UNITTEST_NAME ?= $(UTTEST_NAME_FROM_PATH)
    endif
    ifdef $(BUILD_POC)
      POCNAME_FROM_PATH := $(shell basename $(MY_PATH))
      POC_NAME ?= $(POCNAME_FROM_PATH)
    endif
endif
