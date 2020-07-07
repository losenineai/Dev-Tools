LOCAL_PATH := $(call my-dir)

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


# DxLibc 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXLibc
LOCAL_SRC_FILES := $(LOCAL_PATH)/anti/dx-libc/libs/$(TARGET_ARCH_ABI)/libDxLibc.a
# prebuild static lib
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
# DxLibc end


# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := DxAntiTest
LOCAL_CFLAGS    += -Wall -fpermissive -DHAVE_PTHREAD
LOCAL_CPPFLAGS  += -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--gc-sections

# treat all warning as error, never delete this !
#LOCAL_CFLAGS += -Werror

ifeq ($(NDK_BUILD_CMODE), debug)
	LOCAL_CFLAGS += -DANTI_DEBUG
else
	LOCAL_CFLAGS += -fvisibility=hidden -DANTI_DEBUG
endif

LOCAL_CFLAGS += -DANTI_RETURN_ON

ifeq ($USE_STEE_NDK), true)
    LOCAL_CFLAGS += -xse -bcf=100 -fla=100
endif

ANTI_ROOT_DIR := $(LOCAL_PATH)/anti
TEST_ROOT_DIR := $(LOCAL_PATH)/test

LOCAL_LDLIBS += -llog -lz -latomic
LOCAL_C_INCLUDES +=                                             \
    $(ANTI_ROOT_DIR)                                            \
    $(TEST_ROOT_DIR)                                            \
    $(ANTI_ROOT_DIR)/include                                    \
    $(ANTI_ROOT_DIR)/src/utils/include                          \
    $(ANTI_ROOT_DIR)/dx-crypto/api                              \
    $(ANTI_ROOT_DIR)/dx-crypto/md5                              \
    $(ANTI_ROOT_DIR)/dx-crypto/pkcs7                            \
    $(ANTI_ROOT_DIR)/dx-crypto/zip                              \
    $(ANTI_ROOT_DIR)/dx-hook                                    \
    $(ANTI_ROOT_DIR)/dx-libc                                    \
    $(ANTI_ROOT_DIR)/cutils                                     \
    $(ANTI_ROOT_DIR)/src/emu                                    \
    $(ANTI_ROOT_DIR)/src/hook_checker/include                   \

# Dx-crypto-ext begin
CRYPTO_DIR := $(ANTI_ROOT_DIR)/dx-crypto
CRYPTO_SRC := $(wildcard                                        \
    $(CRYPTO_DIR)/api/*.cc                                      \
    $(CRYPTO_DIR)/md5/*.c                                       \
    $(CRYPTO_DIR)/pkcs7/*.cc                                    \
    $(CRYPTO_DIR)/zip/*.c                                       \
)                                                               \
# Dx-libc-ext end

# Anti begin
ANTI_SRC := $(wildcard                                          \
    $(ANTI_ROOT_DIR)/src/*.cpp                                  \
    $(ANTI_ROOT_DIR)/src/utils/*.cc                             \
    $(ANTI_ROOT_DIR)/src/emu/*.cpp                              \
    $(ANTI_ROOT_DIR)/src/hook_checker/*.cpp                     \
    $(ANTI_ROOT_DIR)/src/hook_checker/*.inc                     \
)                                                               \
# ANti end


# Dx-libc-ext begin
LIBC_EXT_DIR := $(ANTI_ROOT_DIR)/dx-libc-ext
LIBC_EXT_SRC := $(wildcard                                      \
    $(LIBC_EXT_DIR)/*.cc                                        \
)                                                               \
# Dx-libc-ext end

# Test begin
TEST_CODE_SRC := $(wildcard                                     \
    $(TEST_ROOT_DIR)/*.cpp                                      \
    $(TEST_ROOT_DIR)/*.cc                                       \
)                                                               \
# Test end

# cutils begin
CUTILS_SRC := $(wildcard                                        \
    $(ANTI_ROOT_DIR)/cutils/*.c                                 \
)                                                               \
# cutils end

LOCAL_SRC_FILES :=  $(ANTI_SRC:$(LOCAL_PATH)/%=%)               \
                    $(CRYPTO_SRC:$(LOCAL_PATH)/%=%)             \
                    $(LIBC_EXT_SRC:$(LOCAL_PATH)/%=%)           \
                    $(TEST_CODE_SRC:$(LOCAL_PATH)/%=%)          \
                    $(CUTILS_SRC:$(LOCAL_PATH)/%=%)             \

# LOCAL_STATIC_LIBRARIES += DXHook DXCrypto
LOCAL_STATIC_LIBRARIES += DXHook DXLibc

include $(BUILD_SHARED_LIBRARY)



