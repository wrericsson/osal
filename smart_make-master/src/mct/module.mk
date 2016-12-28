###############################################################################

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(ROOT_DIR)/include/mct
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(shell find $(LOCAL_PATH) -name *.c))
LOCAL_MODULE := libmct

include $(BUILD_STATIC_LIBRARY)

###############################################################################
