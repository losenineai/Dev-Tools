//
// Created by white on 2018/5/22.
//

#ifndef ANDROID_DX_ANTI_H
#define ANDROID_DX_ANTI_H

#include <jni.h>
#include <stdint.h>
#include "anti_config.h"
#include "dx_anti_prefix.h"




#ifdef __cplusplus
extern "C" {
#endif

/**
 * 守护进程的反调试检测
 */
#define DXANTI_FLAG_DEBUG       (1)
#define DXANTI_FLAG_EMULATOR    (1 << 1)
#define DXANTI_FLAG_FLAW_JANUS  (1 << 3)
#define DXANTI_FLAG_INJECT      (1 << 4)        // 0x10
#define DXANTI_FLAG_MEMDUMP     (1 << 5)
#define DXANTI_FLAG_MULTIRUN    (1 << 6)
#define DXANTI_FLAG_ROOT        (1 << 7)
#define DXANTI_FLAG_THREAD      (1 << 8)
#define DXANTI_FLAG_LOG         (1 << 9)
//#define DXANTI_FLAG_VIRTUAL_APP (1 << 10)  //将virtual app检测合并到multirun检测中。
#define DXANTI_FLAG_HOOK        (1 << 11)
#define DXANTI_FLAG_JNIHOOK     (1 << 12)
/**
 * 无守护进程的反调试检测
 * 启动必须： DXANTI_FLAG_DEBUG | DXANTI_FLAG_DEBUG_EXT_NO_DAEMON
 */
#define DXANTI_FLAG_DEBUG_EXT_NO_DAEMON (1 << 11)
//#define DXANTI_FLAG_PROXY 1 << 8

/**
 * jni 环境初始化
 * 注意：Anti功能使用前必须调用
 * @param vm
 * @param jContext jni Context实例
 */
void DXAnti_monitor_init(JavaVM *vm, jobject jContext);


/**
 *
 * Anti对抗功能启动，线程/进程不断检测，检测到随机不同崩溃
 * -1 默认为 ALL
 *
 */
void DXAnti_monitor_start(int64_t flag);


/**
 * 插装检测Anti功能
 */
bool DXAnti_monitor_check();


/**
 * 开启anti结果返回功能的宏
 */
#ifdef ANTI_RETURN_ON
typedef void (*DXAnti_ResultCallback)(int64_t flag, void *args);
void DXAnti_set_monitor_callback(DXAnti_ResultCallback callback);
#endif

#ifndef ANDROID_JNI_HOOK
#define ANDROID_JNI_HOOK

typedef struct _DXMethod
{
    const char* clsName;
    const char* name;
    const char* sig;
} DXMethod;

#endif


/**
 *  初始化, 只需调用一将会 。建议放在JNI_OnLoad里。
 * @param vm
 * @param env
 */
void DXAnti_detect_jni_hook_init(JavaVM* vm, JNIEnv* env);

/**
 * 设置重点关注的Java方法列表，只需调用一次。 建议放在JNI_OnLoad里。
 * @param methods
 */
void DXAnti_detect_jni_hook_add_methods(DXMethod methods[], size_t size);

/**
 *  监控jni相关。建议放在每个Jni导出函数里。
 * @param env
 * @param retAddr
 * @return
 */
JNIEnv* DXAnti_detect_jni_hook_setup(JNIEnv *env, uintptr_t retAddr);



#ifdef __cplusplus
}
#endif

#endif //ANDROID_DX_ANTI_H
