LOCAL_PATH := $(call my-dir)

# strip debug
# cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := ndk-utils
LOCAL_CFLAGS    += -Wall -fpermissive -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI
LOCAL_CPPFLAGS  += -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections

# treat all warning as error, never delete this !
LOCAL_CFLAGS += -Werror


# export symbol flag, default hidden all
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS += -llog -lz -latomic


SRC_DIR := $(LOCAL_PATH)/../../src

LOCAL_C_INCLUDES +=                                             \
    $(SRC_DIR)/                                                 \


SRC := $(wildcard                                               \
    $(SRC_DIR)/*.cc                                             \
    $(SRC_DIR)/*.c                                              \
)                                                               \

LOCAL_SRC_FILES :=  $(SRC:$(LOCAL_PATH)/%=%)               \



# build so
#include $(BUILD_SHARED_LIBRARY)

# build a
include $(BUILD_STATIC_LIBRARY)


