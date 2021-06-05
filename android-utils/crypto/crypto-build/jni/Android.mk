LOCAL_PATH := $(call my-dir)

# strip debug
cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := DxCrypto
LOCAL_CFLAGS    += -Wall -fpermissive -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI
LOCAL_CPPFLAGS  += -std=gnu++11 -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--gc-sections

# treat all warning as error, never delete this !
# LOCAL_CFLAGS += -Werror


# LOCAL_CFLAGS += -stee-config=dx-config.yaml

# export symbol flag, default hidden all
LOCAL_CFLAGS += -fvisibility=hidden

# LOCAL_LDLIBS += -llog -lz -latomic

CRYPTO_DIR := $(LOCAL_PATH)/src

CRYPTO_INCLUDES :=                                                    \
          $(CRYPTO_DIR)                                               \
          $(CRYPTO_DIR)/dx_headers                                    \
          $(CRYPTO_DIR)/dx_tools                                      \
          $(CRYPTO_DIR)/base64                                        \
          $(CRYPTO_DIR)/md5                                           \
          $(CRYPTO_DIR)/aes                                           \
          $(CRYPTO_DIR)/sha1                                          \
          $(CRYPTO_DIR)/sha256                                        \
          $(CRYPTO_DIR)/sm3                                           \
          $(CRYPTO_DIR)/sm4                                           \
          $(CRYPTO_DIR)/rc4                                           \
          $(CRYPTO_DIR)/wbaes                                         \
          $(CRYPTO_DIR)/xxtea                                         \
          $(CRYPTO_DIR)/mt                                            \


CRYPTO_SRC := $(wildcard $(CRYPTO_DIR)/*.cc                           \
          $(CRYPTO_DIR)/dx_api/*.cc                                   \
          $(CRYPTO_DIR)/dx_tools/*.cc                                 \
          $(CRYPTO_DIR)/base64/*.c                                    \
          $(CRYPTO_DIR)/md5/*.c                                       \
          $(CRYPTO_DIR)/aes/*.c                                       \
          $(CRYPTO_DIR)/sha1/*.c                                      \
          $(CRYPTO_DIR)/sha256/*.c                                    \
          $(CRYPTO_DIR)/sm3/*.c                                       \
          $(CRYPTO_DIR)/sm4/*.c                                       \
          $(CRYPTO_DIR)/rc4/*.c                                       \
          $(CRYPTO_DIR)/wbaes/*.c                                     \
          $(CRYPTO_DIR)/xxtea/*.c                                     \
          $(CRYPTO_DIR)/mt/*.c                                        \
         )                                                            \


LOCAL_C_INCLUDES +=                                                   \
    $(CRYPTO_INCLUDES)                                                \


LOCAL_SRC_FILES :=                                                    \
              $(CRYPTO_SRC:$(LOCAL_PATH)/%=%)                         \
              $(CORE_SRC:$(LOCAL_PATH)/%=%)                           \

# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
# include $(BUILD_EXECUTABLE)
