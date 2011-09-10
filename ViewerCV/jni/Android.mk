#
#
#
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#pass in OPENCV_ROOT or define it here
OPENCV_ROOT := ../android-opencv/opencv

#define OPENCV_INCLUDES
include $(OPENCV_ROOT)/includes.mk

#define OPENCV_LIBS
include $(OPENCV_ROOT)/libs.mk

LOCAL_LDLIBS += $(OPENCV_LIBS) $(ANDROID_OPENCV_LIBS) -llog -lGLESv2	
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
    
LOCAL_C_INCLUDES +=  $(OPENCV_INCLUDES) $(ANDROID_OPENCV_INCLUDES)

LOCAL_CFLAGS += -O3

LOCAL_MODULE := cvcamera

### Add all source file names to be included in lib separated by a whitespace
LOCAL_SRC_FILES :=  Processor.cpp cvcamera_jni.cpp

include $(BUILD_SHARED_LIBRARY)
