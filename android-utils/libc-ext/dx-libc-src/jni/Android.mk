LOCAL_PATH := $(call my-dir)

# strip debug
# cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := DxLibc
LOCAL_CFLAGS    += -fPIC -Wall -fpermissive -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI 
# LOCAL_CFLAGS += -O3 -fpie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
# LOCAL_CPPFLAGS += -O3 -fpie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-exceptions -fno-rtti -fno-unwind-tables
LOCAL_CPPFLAGS  += -frtti -w #-fPIC
# LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel

ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
    LOCAL_C_INCLUDES += $(NDK_ROOT)/sysroot/usr/include/aarch64-linux-android
else ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_C_INCLUDES += $(NDK_ROOT)/sysroot/usr/include/arm-linux-androideabi
else ifeq ($(TARGET_ARCH_ABI), x86)
    LOCAL_C_INCLUDES += $(NDK_ROOT)/sysroot/usr/include/i686-linux-android
else ifeq ($(TARGET_ARCH_ABI), x86_64)
    LOCAL_C_INCLUDES += $(NDK_ROOT)/sysroot/usr/include/x86_64-linux-android
else ifeq ($(TARGET_ARCH_ABI), armeabi)
    LOCAL_C_INCLUDES += $(NDK_ROOT)/sysroot/usr/include/arm-linux-androideabi
endif


# $(warning "the value of ANDROID_NDK_HOME is$(LOCAL_C_INCLUDES)")
# $(warning "the value of NDK_ROOT is$(NDK_ROOT)")
# export symbol flag, default hidden all
# LOCAL_CFLAGS += -fvisibility=hidden

DXLIBC_SRC := $(wildcard                                        \
    $(LOCAL_PATH)/log/*.cpp                                     \
    $(LOCAL_PATH)/elf/*.cpp                                    \
    $(LOCAL_PATH)/stdio/*.cpp                                   \
    $(LOCAL_PATH)/stdio/*.c   \
    $(LOCAL_PATH)/string/*.c                                    \
    $(LOCAL_PATH)/string/*.cpp                                    \
    $(LOCAL_PATH)/syscall/private/*.cpp                     \
    $(LOCAL_PATH)/stdio/dx_gdtoa/*.c                     \
    $(LOCAL_PATH)/stdio/dx_gdtoa/*.cpp                     \
    $(LOCAL_PATH)/thread/*.cpp                     \
    $(LOCAL_PATH)/properties/*.cpp                     \
)                                                               \

ifeq ($(TARGET_ARCH_ABI), armeabi)
    DXLIBC_SRC += $(wildcard                            \
        $(LOCAL_PATH)/syscall/armeabi-v7a/*.S           \
    )
else
    DXLIBC_SRC += $(wildcard                            \
        $(LOCAL_PATH)/syscall/$(TARGET_ARCH_ABI)/*.S    \
    )
endif

LOCAL_SRC_FILES :=  $(DXLIBC_SRC)

LOCAL_STATIC_LIBRARIES += DxLibc

# build so
# include $(BUILD_SHARED_LIBRARY)

# build .a
include $(BUILD_STATIC_LIBRARY)

