LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#IS_DEBUG := true
UTILS_DIR := ../../utils/
LOCAL_MODULE    := antidump-shellcode
LOCAL_CFLAGS += -O3 -fpie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
  	SYSCALL_SRC=$(UTILS_DIR)/syscall/syscall-arm64.S
	LOCAL_CFLAGS += -DM_ARM64
else ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	SYSCALL_SRC=$(UTILS_DIR)/syscall/syscall-armeabi-v7a.S
	LOCAL_CFLAGS += -DM_ARM
else ifeq ($(TARGET_ARCH_ABI), x86)
	SYSCALL_SRC=$(UTILS_DIR)/syscall/syscall-x86.S
	LOCAL_CFLAGS += -DM_X86
endif

ifdef IS_DEBUG
	LOCAL_CFLAGS += -DDEBUG
endif

LOCAL_LDFLAGS += -llog
LOCAL_SRC_FILES := integrityCheck.c $(UTILS_DIR)func_call.c $(UTILS_DIR)libc_call.c $(UTILS_DIR)dead_call.c $(SYSCALL_SRC)

ifdef IS_DEBUG
	LOCAL_SRC_FILES += $(UTILS_DIR)liblog_call.c
endif

# LOCAL_SRC_FILES += $(UTILS_DIR)syscall.S
include $(BUILD_SHARED_LIBRARY)
# include $(BUILD_EXECUTABLE)