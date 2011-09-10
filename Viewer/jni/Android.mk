#
#
#
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -O3

LOCAL_MODULE    := ViewerN

LOCAL_SRC_FILES := com_android_viewer_imageprocessing_native.c

include $(BUILD_SHARED_LIBRARY)
