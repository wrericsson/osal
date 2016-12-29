###############################################################################

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS  += -Werror
LOCAL_CFLAGS  += -LOCAL_CFLAGS
LOCAL_LDFLAGS += -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=realloc --Wl,--wrap=open  Wl,--wrap=calloc

LOCAL_C_INCLUDES += $(ROOT_DIR)/include/memleak 
LOCAL_SRC_DIR := $(LOCAL_PATH)
LOCAL_SRC_FILES += $(shell find $(LOCAL_SRC_DIR) -name '*.c' | sed s:^$(LOCAL_PATH)/::g )
LOCAL_MODULE := libmemleak

include $(BUILD_STATIC_LIBRARY)

###############################################################################
