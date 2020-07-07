LOCAL_PATH := $(call my-dir)

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# DXHook 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXHook
LOCAL_SRC_FILES := $(LOCAL_PATH)/dx-hook/libs/$(TARGET_ARCH_ABI)/libDxHook.a
include $(PREBUILT_STATIC_LIBRARY)
# DXHook end

# DXAnti 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXAnti
LOCAL_SRC_FILES := $(LOCAL_PATH)/dx-anti/libs/$(TARGET_ARCH_ABI)/libDxAnti.a
include $(PREBUILT_STATIC_LIBRARY)
# DXAnti end

# DxLibc 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXLibc
LOCAL_SRC_FILES := $(LOCAL_PATH)/dx-libc/libs/$(TARGET_ARCH_ABI)/libDxLibc.a
# prebuild static lib
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
# DxLibc end




# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := DxAntiDemo
LOCAL_CFLAGS	+= -Wall -fpermissive -DHAVE_PTHREAD
LOCAL_CPPFLAGS  += -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--gc-sections

# treat all warning as error, never delete this !
#LOCAL_CFLAGS += -Werror

LOCAL_CFLAGS += -fvisibility=hidden -DENABLE_RISKMGR -DENABLE_CHECKMGR -DENABLE_SAFE

ifeq ($USE_STEE_NDK), true)
    LOCAL_CFLAGS += -xse -bcf=100 -fla=100
endif

LOCAL_LDLIBS += -llog -lz -latomic
LOCAL_C_INCLUDES +=                                             \
    $(LOCAL_PATH)                                               \
    $(LOCAL_PATH)/dx-anti                                       \
    $(LOCAL_PATH)/dx-hook                                       \

# Test begin
CORE_SRC := $(wildcard  $(LOCAL_PATH)/*.cpp  $(LOCAL_PATH)/*.cc)\
# Test end

LOCAL_SRC_FILES :=  $(CORE_SRC:$(LOCAL_PATH)/%=%)               \

LOCAL_STATIC_LIBRARIES += DXAnti DXHook DXLibc

include $(BUILD_SHARED_LIBRARY)



