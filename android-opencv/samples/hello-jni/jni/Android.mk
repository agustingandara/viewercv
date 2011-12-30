# date: Summer, 2010 
# author: Ethan Rublee
# contact: ethan.rublee@gmail.com
#Android.mk for the hello-jni opencv sample
#please define OPENCV_ROOT when calling ndk-build
#ndk-build OPENCV_ROOT=../../opencv
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_ROOT must be defined.
ifndef OPENCV_ROOT
$(warning Please define OPENCV_ROOT to point to the root folder of opencv, try ndk-build OPENCV_ROOT=../../opencv)
OPENCV_ROOT=../../opencv
$(warning Defaulting to OPENCV_ROOT=$(OPENCV_ROOT))
endif


#define OPENCV_INCLUDES
include $(OPENCV_ROOT)/includes.mk
#define OPENCV_LIBS
include $(OPENCV_ROOT)/libs.mk



LOCAL_LDLIBS += $(OPENCV_LIBS) -llog
    
LOCAL_C_INCLUDES +=  $(OPENCV_INCLUDES)  

LOCAL_CPP_EXTENSION := cc
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.cc

include $(BUILD_SHARED_LIBRARY)
