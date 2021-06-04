LOCAL_PATH := $(call my-dir)

# strip debug
# cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# DXHook 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXHook
LOCAL_SRC_FILES := $(LOCAL_PATH)/anti/dx-hook/libs/$(TARGET_ARCH_ABI)/libDxHook.a
# prebuild static lib
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
# DXAuth end


#  # DxCrypto 配置
#  include $(CLEAR_VARS)
#  LOCAL_MODULE    := DXCrypto
#  LOCAL_SRC_FILES := $(LOCAL_PATH)//anti/dx-crypto/libs/$(TARGET_ARCH_ABI)/libDxCrypto.a
#  # prebuild static lib
#  include $(PREBUILT_STATIC_LIBRARY)
#  include $(CLEAR_VARS)
#  # DxCrypto end


# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := DxAnti
LOCAL_CFLAGS    += -Wall -fpermissive -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI
LOCAL_CPPFLAGS  += -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections

# treat all warning as error, never delete this !
#LOCAL_CFLAGS += -Werror


ifeq ($(USE_STEE_NDK), true)
    $(warning USE_STEE_NDK:$(USE_STEE_NDK))
    LOCAL_CFLAGS += -stee-config=$(LOCAL_PATH)/dx-config.yaml
else
    LOCAL_CFLAGS += -DANTI_DEBUG 
    # LOCAL_CFLAGS += -DANTI_NO_PREFIX
endif

# todo open debug log
# LOCAL_CFLAGS += -DANTI_DEBUG 

# export symbol flag, default hidden all
LOCAL_CFLAGS += -fvisibility=hidden

# for risk
ifeq ($(NO_CRASH), true)
    $(warning NO_CRASH:$(NO_CRASH))
    LOCAL_CFLAGS += -DANTI_RETURN_ON
endif

ANTI_ROOT_DIR := $(LOCAL_PATH)/anti

LOCAL_C_INCLUDES +=                                             \
    $(ANTI_ROOT_DIR)                                            \
    $(ANTI_ROOT_DIR)/include                                    \
    $(ANTI_ROOT_DIR)/src/utils/include                          \
    $(ANTI_ROOT_DIR)/dx-crypto/api                              \
    $(ANTI_ROOT_DIR)/dx-crypto/md5                              \
    $(ANTI_ROOT_DIR)/dx-crypto/pkcs7                            \
    $(ANTI_ROOT_DIR)/dx-crypto/zip                              \
    $(ANTI_ROOT_DIR)/dx-hook                                    \
    $(ANTI_ROOT_DIR)/dx-libc-ext                                \
    $(ANTI_ROOT_DIR)/cutils                                     \
    $(ANTI_ROOT_DIR)/src/emu                                    \


# Anti begin
ANTI_SRC := $(wildcard                                          \
    $(ANTI_ROOT_DIR)/src/*.cpp                                  \
    $(ANTI_ROOT_DIR)/src/utils/*.cc                             \
    $(ANTI_ROOT_DIR)/src/emu/*.cpp                              \
)                                                               \
# ANti end

# Dx-crypto begin
CRYPTO_DIR := $(ANTI_ROOT_DIR)/dx-crypto
CRYPTO_SRC := $(wildcard                                        \
    $(CRYPTO_DIR)/api/*.cc                                      \
    $(CRYPTO_DIR)/md5/*.c                                       \
    $(CRYPTO_DIR)/pkcs7/*.cc                                    \
    $(CRYPTO_DIR)/zip/*.c                                       \
)                                                               \
# Dx-crypto end


# Dx-libc-ext begin
LIBC_EXT_DIR := $(ANTI_ROOT_DIR)/dx-libc-ext
LIBC_EXT_SRC := $(wildcard                                      \
    $(LIBC_EXT_DIR)/*.cc                                        \
)                                                               \
# Dx-libc-ext end

# cutils begin
CUTILS_SRC := $(wildcard                                        \
    $(ANTI_ROOT_DIR)/cutils/*.c                                 \
)                                                               \
# cutils end

LOCAL_SRC_FILES :=  $(ANTI_SRC:$(LOCAL_PATH)/%=%)               \
                    $(LIBC_EXT_SRC:$(LOCAL_PATH)/%=%)           \
                    $(CUTILS_SRC:$(LOCAL_PATH)/%=%)             \
                    $(CRYPTO_SRC:$(LOCAL_PATH)/%=%)             \

# LOCAL_ARM_MODE := arm

# LOCAL_STATIC_LIBRARIES += DXHook DXCrypto
LOCAL_STATIC_LIBRARIES += DXHook

# build so
#include $(BUILD_SHARED_LIBRARY)

# build a
include $(BUILD_STATIC_LIBRARY)


