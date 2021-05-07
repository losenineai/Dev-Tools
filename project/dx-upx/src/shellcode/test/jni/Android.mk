LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#IS_DEBUG := true
UTILS_DIR := ../../utils/
LOCAL_MODULE    := antidump-shellcode
LOCAL_CFLAGS += -O3 -fpie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables


LOCAL_LDFLAGS += -llog
LOCAL_SRC_FILES := test.cpp 


# LOCAL_SRC_FILES += $(UTILS_DIR)syscall.S
# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)