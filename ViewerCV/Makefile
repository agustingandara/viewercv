#load environment from local.env.mk
LOCAL_ENV_MK=local.env.mk
ifneq "$(wildcard $(LOCAL_ENV_MK))" ""
include $(LOCAL_ENV_MK)
else
$(shell cp sample.$(LOCAL_ENV_MK) $(LOCAL_ENV_MK))
$(info local environement not setup! try:)
$(info gedit $(LOCAL_ENV_MK))
$(info Please setup the $(LOCAL_ENV_MK) - the default was just created')
include $(LOCAL_ENV_MK)
endif

# The path to the NDK, requires crystax version r-4 for now, due to support
#for the standard library
ifndef ANDROID_NDK_ROOT
$(error ANDROID_NDK_ROOT not defined, please export this variable or define it before executing make)
endif

ANDROID_NDK_BASE = $(ANDROID_NDK_ROOT)

#define OPENCV_ROOT when calling this makefile
#OPENCV_ROOT = $(ANDROID_NDK_BASE)/apps/opencv
ifndef OPENCV_ROOT
$(error Please define OPENCV_ROOT with something like the command \
make OPENCV_ROOT=<opencv>)
endif

ifndef PROJECT_PATH
$(info PROJECT_PATH defaulting to this directory)
PROJECT_PATH=.
endif

$(info OPENCV_ROOT = $(OPENCV_ROOT))

# The name of the native library
LIBNAME = libcvcamera.so

# Find all the C++ sources in the native folder
SOURCES = $(wildcard jni/*.cpp)
HEADERS = $(wildcard jni/*.h)

ANDROID_MKS = $(wildcard jni/*.mk)

# The real native library stripped of symbols
LIB		= libs/armeabi-v7a/$(LIBNAME) libs/armeabi/$(LIBNAME)

all:	$(LIB)
	ndk-build V=1 

#calls the ndk-build script, passing it OPENCV_ROOT and OPENCV_LIBS_DIR
$(LIB):  $(SOURCES) $(HEADERS) $(ANDROID_MKS)
	$(ANDROID_NDK_BASE)/ndk-build OPENCV_ROOT=$(OPENCV_ROOT) \
	OPENCV_LIBS_DIR=$(OPENCV_LIBS_DIR) PROJECT_PATH=$(PROJECT_PATH) V=$(V) $(NDK_FLAGS)

	
#clean targets
.PHONY: clean cleanall


