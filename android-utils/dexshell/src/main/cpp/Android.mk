LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := csn
LOCAL_CFLAGS	+= -Wall -Wno-unused-function -fvisibility=hidden -Werror # -DSTACK
LOCAL_CPPFLAGS	+= -Wno-unused-function -fvisibility=hidden
LOCAL_LDLIBS += -llog -lz
LOCAL_SRC_FILES := jni-entry.cc HookUtils.cc ReadFromApk.cc decrypt.cc HookProcs.cc common.cc \
                    inject/ElfUtils.cc \
                    zip/zip.c \
 					zip/unzip.c \
 					zip/ioapi.c \
 					zip/ioapi_buf.c \
 					zip/ioapi_mem.c

ifneq ($(ENABLE_ASAN),)
    LOCAL_ARM_MODE := arm
    LOCAL_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
    LOCAL_LDFLAGS += -fsanitize=address
endif

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := csn2
LOCAL_CFLAGS	+= -Wall -Wno-unused-function -fvisibility=hidden -Werror # -DSTACK
LOCAL_CPPFLAGS	+= -Wno-unused-function -fvisibility=hidden
LOCAL_LDLIBS += -llog
LOCAL_SRC_FILES := HookUtils.cc inject/inject.cc inject/ElfUtils.cc common.cc decrypt.cc

include $(BUILD_SHARED_LIBRARY)
