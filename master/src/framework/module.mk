###############################################################################

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(ROOT_DIR)/include/hal 
LOCAL_C_INCLUDES += $(ROOT_DIR)/include/osal 
LOCAL_SRC_FILES := framework.c main.c
LOCAL_MODULE := libframework

include $(BUILD_STATIC_LIBRARY)

###############################################################################
