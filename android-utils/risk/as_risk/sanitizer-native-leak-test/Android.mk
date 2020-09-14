LOCAL_PATH := $(call my-dir)

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


# DXHook 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXHook
LOCAL_SRC_FILES := $(LOCAL_PATH)/android/dx-libs/dx-hook/libs/$(TARGET_ARCH_ABI)/libDxHook.a
include $(PREBUILT_STATIC_LIBRARY)
# DXHook end

# DXAnti 配置
include $(CLEAR_VARS)
LOCAL_MODULE    := DXAnti
LOCAL_SRC_FILES := $(LOCAL_PATH)/android/dx-libs/dx-anti/libs/$(TARGET_ARCH_ABI)/libDxAnti.a
include $(PREBUILT_STATIC_LIBRARY)
# DXAnti end


# ************************** DXRisk Version **************************
include $(CLEAR_VARS)
ANDROID_ROOT_PATH := $(shell cd ../; pwd)
HAVE_PROPERTIES_FILE := $(shell test -f $(ANDROID_ROOT_PATH)/gradle.properties && echo yes)

ifneq ($(HAVE_PROPERTIES_FILE), yes)
ANDROID_ROOT_PATH := $(shell cd ../../../../; pwd)
endif

RISK_VERSION=$(shell (cat $(ANDROID_ROOT_PATH)/gradle.properties | grep "dx_sdk_version" | cut -d= -f2))

# ************************** customize config **************************
COMPANY=$(shell (cat $(ANDROID_ROOT_PATH)/sdk-customize.properties | grep "company" | cut -d= -f2))
PREFIX=$(shell (echo ${COMPANY} | tr 'a-z' 'A-Z'))
$(warning "PREFIX is $(PREFIX)")

# ************************** DXRisk Config **************************
include $(CLEAR_VARS)
LOCAL_MODULE    := ${PREFIX}RiskComm-${RISK_VERSION}
LOCAL_CFLAGS	+= -fpermissive -DHAVE_PTHREAD -D__STDC_FORMAT_MACROS
LOCAL_CPPFLAGS  += -frtti -D__STDC_FORMAT_MACROS

CUSTOMIZE_DXRISK_INTERNAL=$(shell (cat $(ANDROID_ROOT_PATH)/sdk-compile/sdk-config/natives.cfg | grep "DXRiskInternal" | cut -d' ' -f2))
CUSTOMIZE_STORAGE_UTILS=$(shell (cat $(ANDROID_ROOT_PATH)/sdk-compile/sdk-config/natives.cfg | grep "StorageUtils" | cut -d' ' -f2))
CUSTOMIZE_JNIHELPER=$(shell (cat $(ANDROID_ROOT_PATH)/sdk-compile/sdk-config/natives.cfg | grep "JNIHelper" | cut -d' ' -f2))

LOCAL_CFLAGS	+= -DCUSTOMIZE_DXRISK_INTERNAL="\"$(CUSTOMIZE_DXRISK_INTERNAL)"\"
LOCAL_CFLAGS	+= -DCUSTOMIZE_STORAGE_UTILS="\"${CUSTOMIZE_STORAGE_UTILS}"\"
LOCAL_CFLAGS	+= -DCUSTOMIZE_JNIHELPER="\"${CUSTOMIZE_JNIHELPER}"\"

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections -D__STDC_FORMAT_MACROS
LOCAL_CFLAGS += -ffunction-sections -fdata-sections -D__STDC_FORMAT_MACROS
LOCAL_LDFLAGS += -Wl,--gc-sections

ifeq ($(NDK_DXRISK_CMODE), debug)
	LOCAL_CFLAGS += -DINFO_LOG -DDXDEBUG -DDEBUG
else
	LOCAL_CFLAGS += -fvisibility=hidden
endif

ifeq ($(USE_STEE_NDK), true)
    # LOCAL_CFLAGS += -steec-extra-opts=-xse,-bcf=100,-fla=100,-sub=100
    LOCAL_CFLAGS += -stee-config=dx-config.yaml
    # $(warning $(USE_STEE_NDK))
endif

LOCAL_LDLIBS += -llog -lz -latomic


# dx-uuid
UUID_DIR := $(LOCAL_PATH)/android/dx-uuid
UUID_INCLUDE := $(UUID_DIR)
UUID_SRC := $(wildcard $(UUID_DIR)/*.c)


LOCAL_C_INCLUDES +=                                     \
    $(LOCAL_PATH)/core/include                          \
    $(LOCAL_PATH)/core/src                              \
    $(LOCAL_PATH)/core/src/thirdpart                    \
    $(LOCAL_PATH)/core/src/thirdpart/base64             \
    $(LOCAL_PATH)/core/src/thirdpart/json               \
    $(LOCAL_PATH)/core/src/thirdpart/md5                \
    $(LOCAL_PATH)/core/src/thirdpart/minicrypt          \
    $(LOCAL_PATH)/core/src/thirdpart/sha1               \
    $(LOCAL_PATH)/core/src/thirdpart/sm3                \
    $(LOCAL_PATH)/core/src/thirdpart/xxtea              \
    $(LOCAL_PATH)/core/src/thirdpart/zip                \
    $(LOCAL_PATH)/core/src/protobuf-c                   \
    $(LOCAL_PATH)/core/src/pb                           \
    $(LOCAL_PATH)/android/dx-libs/dx-anti               \
    $(UUID_INCLUDE)                                     \

# PBLite begin
PBLITE_SRC_DIR := $(LOCAL_PATH)/core/src/protobuf-c
PBLITE_SRC :=     $(PBLITE_SRC_DIR)/*.c

# stee pb src begin
STEE_PB_SRC := $(wildcard $(LOCAL_PATH)/core/src/pb/*.c)
# stee pb src end

# 3rdpart begin
THIRDPART_SRC_DIR := $(LOCAL_PATH)/core/src/thirdpart
THIRDPART_SRC := $(wildcard                         \
                $(THIRDPART_SRC_DIR)/base64/*.c     \
                $(THIRDPART_SRC_DIR)/json/*.cpp     \
                $(THIRDPART_SRC_DIR)/md5/*.c        \
                $(THIRDPART_SRC_DIR)/minicrypt/*.c  \
                $(THIRDPART_SRC_DIR)/sha1/*.c       \
                $(THIRDPART_SRC_DIR)/sm3/*.c        \
                $(THIRDPART_SRC_DIR)/xxtea/*.c      \
                $(THIRDPART_SRC_DIR)/zip/*.c        \
                )                                   \
# 3rdpart end

# android impl begin
ANDROID_IMPL_SRC := $(wildcard $(LOCAL_PATH)/android/*.cc   \
                        $(LOCAL_PATH)/android/*.c           \
                     )                                      \
# android impl end

# core begin
CORE_SRC := $(wildcard $(LOCAL_PATH)/core/src/*.cc )
# core eng

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := 	$(PBLITE_SRC)                           \
                    $(STEE_PB_SRC:$(LOCAL_PATH)/%=%)        \
					$(THIRDPART_SRC:$(LOCAL_PATH)/%=%)      \
					$(ANDROID_IMPL_SRC:$(LOCAL_PATH)/%=%)   \
					$(CORE_SRC:$(LOCAL_PATH)/%=%)           \
					$(UUID_SRC:$(LOCAL_PATH)/%=%)           \


$(warning LOCAL_CFLAGS: $(LOCAL_CFLAGS))

LOCAL_STATIC_LIBRARIES += DXAnti DXHook

include $(BUILD_SHARED_LIBRARY)


