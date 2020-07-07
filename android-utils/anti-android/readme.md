# Anti项目 接入说明
## 目录说明
* `anti-as-project` android studio anti项目
* `anti-build` ndk-build编译目录
* `anti-headers` anti 头文件
* `anti-src` anti核心源码
* `readme.md` 说明文档

## 源码说明
* `anti-src/src`, `anti-src/include` anti源代码
* `anti-src/xxx` 第三方源码xxx

## makefile配置

```  
# Anti debug日志开启
ifeq ($(NDK_BUILD_CMODE), debug)
    LOCAL_CFLAGS += -DANTI_DEBUG
endif

# Anti 源文件
# Anti begin
ANTI_ROOT_DIR := $(LOCAL_PATH)/anti
LOCAL_C_INCLUDES +=                                             \
    $(ANTI_ROOT_DIR)                                            \
    $(ANTI_ROOT_DIR)/include                                    \
    $(ANTI_ROOT_DIR)/pkcs7                                      \
    $(ANTI_ROOT_DIR)/src/utils/include                          \
    $(ANTI_ROOT_DIR)/dx-crypto                                  \
    $(ANTI_ROOT_DIR)/dx-crypto/md5                              \
    $(ANTI_ROOT_DIR)/dx-crypto/zip                              \
    $(ANTI_ROOT_DIR)/dx-hook                                    \
    $(ANTI_ROOT_DIR)/dx-libc-ext                                \
    $(ANTI_ROOT_DIR)/cutils                                     \
    $(ANTI_ROOT_DIR)/src/emu                                    \

ANTI_SRC := $(wildcard                                          \
    $(ANTI_ROOT_DIR)/src/*.cpp                                  \
    $(ANTI_ROOT_DIR)/pkcs7/*.cpp                                \
    $(ANTI_ROOT_DIR)/src/utils/*.cc                             \
    $(ANTI_ROOT_DIR)/src/emu/*.cpp                              \
)                                                               \                                        
# ANti end
                

```


## API：初始化

```
#include "dx_anti.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    DXAnti_jni_init(vm);
    return JNI_VERSION_1_4;
}
```

## API: Anti功能

```
#include "dx_anti.h"

void main(){
    // 注入检测
    DXAnti_detect_inject();
    // 内存dump检测
    DXAnti_detect_memdump();
    // debug检测
    DXAnti_detect_debug();
    // 多开检测
    DXAnti_detect_multirun();
    // 非法apk检测
    DXAnti_detect_flaw_janus();
    // root检测
    DXAnti_detect_root();
    
 
    /**
     * Anti对抗功能启动，线程不断检测，检测到随机不同崩溃
     * @param flag 默认0，全部项检测
     *
     *   FLAG_DEBUG = 1;
     *   FLAG_FLAW_JANUS = 1 << 1;
     *   FLAG_INJECT = 1 << 2;
     *   FLAG_MEMdDUMP = 1 << 3;
     *   FLAG_MULTIRUN = 1 << 4;
     *   FLAG_ROOT = 1 << 5;
     *
     *   除以上其他值，默认为FLAG_ALL
     */
    DXAnti_monitor_start(FLAG_DEBUG | FLAG_FLAW_JANUS | FLAG_INJECT | FLAG_MEMDUMP | FLAG_MULTIRUN);
}

```

*created by white 
2018.5.28*


