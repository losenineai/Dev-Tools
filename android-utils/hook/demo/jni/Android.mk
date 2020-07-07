LOCAL_PATH := $(call my-dir)  
 
include $(CLEAR_VARS)
LOCAL_MODULE    := hook
ifeq ($(TARGET_ARCH), arm)
	LOCAL_SRC_FILES := ./../../../../utils/dx-hook/out/libs/armeabi-v7a/libDxHook.a
else ifeq ($(TARGET_ARCH), arm64)
	LOCAL_SRC_FILES := ./../../../../utils/dx-hook/out/libs/arm64-v8a/libDxHook.a
else ifeq ($(TARGET_ARCH), x86)
	LOCAL_SRC_FILES := ./../../../../utils/dx-hook/out/libs/x86/libDxHook.a
else ifeq ($(TARGET_ARCH), x86_64)
	LOCAL_SRC_FILES := ./../../../../utils/dx-hook/out/libs/x86_64/libDxHook.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := test_hook  
LOCAL_SRC_FILES := hello.cpp \
					base64.c \
					TestFunc1.c \
					TestFunc2.c \
					TestFunc3.c \
					xxtea.c

LOCAL_C_INCLUDES += ./../../../utils/dx-hook/out

# LOCAL_ARM_MODE := arm  
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_STATIC_LIBRARIES := hook

# include $(BUILD_SHARED_LIBRARY)
# include $(BUILD_STATIC_LIBRARY)
include $(BUILD_EXECUTABLE)
