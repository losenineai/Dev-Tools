LOCAL_PATH := $(call my-dir)  
  


#  include $(CLEAR_VARS)
# LOCAL_MODULE    := hook
# LOCAL_SRC_FILES := ./../../substrate/obj/local/armeabi-v7a/libhook_hello_test.a

# include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)  
#LOCAL_ARM_MODE := arm  

ifdef DX_HOOK_PREFIX
	LOCAL_CFLAGS += -DDX_HOOK_PREFIX=$(DX_HOOK_PREFIX)
$(warning 'hehe fuck')
$(warning $(DX_HOOK_PREFIX))
else
$(warning 'haha fuck')
endif

LOCAL_MODULE    := DxHook  
LOCAL_SRC_FILES := 	SubstrateDebug.cpp 						\
					SubstrateHook.cpp 						\
					SubstratePosixMemory.cpp 				\
					hde64.cpp 								\
					utils.cpp 								\
					elf_reader.cpp							\

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include

LOCAL_CFLAGS    := -Werror -fvisibility=hidden
LOCAL_CPPFLAGS += -fvisibility=hidden

#LOCAL_CFLAGS += -fopenmp=libomp
#LOCAL_CPPFLAGS += -fopenmp=libomp

# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
# include $(BUILD_EXECUTABLE)

