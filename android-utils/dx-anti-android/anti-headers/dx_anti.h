//
// Created by white on 2018/5/22.
//

#ifndef ANDROID_DX_ANTI_H
#define ANDROID_DX_ANTI_H

#include <string>
#include <stdint.h>
#include <jni.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define DXAnti_monitor_init                                             I9lL00LO0ig0OI0iligI
#define DXAnti_monitor_start                                            OiilIgl9lil0O1iIIlO0
#define DXAnti_monitor_check                                            g1il91L09l91IL9O9Ll9

#define DXAnti_detect_debug_entry                                       OlOiioi09oo0IlglIolL
#define DXAnti_detect_emulator_entry                                    ggoO0oiILiO19IoOii1l
#define DXAnti_detect_flaw_janus_entry                                  IOO9I0iOo0iIlgigi91i
#define DXAnti_detect_inject_entry                                      gl0oiL9IgIg1Li9igOli
#define DXAnti_detect_memdump_entry                                     lLiLIIiiio0Oo0LgIIli
#define DXAnti_detect_multirun_entry                                    LOOOOg90IgOl9gglIO91
#define DXAnti_detect_root_entry                                        l0O1g01llI1LIl0o9Oli
#define DXAnti_detect_thread_entry                                      l1i9I1IoLg0o01i910Lg
#define DXAnti_detect_log_entry                                         i1ool1OO11iig1g9000i
#define DXAnti_detect_virtual_app_entry                                 gLi1Oo09Oo99l1iI1LiI
#define DXAnti_detect_debug_inner_entry                                 iIll119LiiI1gliLgOgi
#define DXAnti_detect_jni_hook_entry									iIll119LiiI1gliLgG1l

#define DXAnti_verify_pkg                                               go0O9o10LLgiOIIILilL
#define DXAnti_verify_signature                                         gL1LIL1lo0iil0Il9Oo0
#define DXAnti_set_monitor_callback                                     O0i0Og9l0OOl0LlLiO11


// dx_jni_hook
#define DXAnti_detect_jni_hook_init                                     o9i9g091ggli1OlLll11
#define DXAnti_detect_jni_hook_add_methods                              o9i9g091ggli1OlLll1l
#define DXAnti_detect_jni_hook_setup                                    o9i9g091ggli1OlLlll1

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


/**
 * jni 环境初始化
 * 注意：Anti功能使用前必须调用
 * @param vm
 * @param jContext jni Context实例
 */
void DXAnti_monitor_init(JavaVM *vm, jobject jContext);


/**
 * Anti对抗功能启动，线程不断检测，检测到随机不同崩溃
 * @param flag
 *
 */
void DXAnti_monitor_start(int64_t flag);


/**
 * 插装检测Anti功能
 */
bool DXAnti_monitor_check();

/**
 * 调试检测，守护进程
 * @return
 */
void DXAnti_detect_debug_entry();

/**
 * 调试检测，单进程
 * @return
 */
void DXAnti_detect_debug_inner_entry();

/**
 * 调试模拟器
 * @return
 */
void DXAnti_detect_emulator_entry();

/**
 * apk 非法检测
 * @return
 */
void DXAnti_detect_flaw_janus_entry();

/**
 * hook注入检测
 * @return
 */
void DXAnti_detect_inject_entry();

/**
 * 内存 dump 检测
 * @return
 */
void DXAnti_detect_memdump_entry();

/**
 * 多开检测
 * @return
 */
void DXAnti_detect_multirun_entry();

/**
 * root 检测
 * @return
 */
void DXAnti_detect_root_entry();

/**
 * 监控线程
 * @return
 */
void DXAnti_detect_thread_entry();

/**
 * 清除日志
 * @return
 */
bool DXAnti_detect_log_entry();

/**
 * 检测virtual app
 * @return
 */
// void DXAnti_detect_virtual_app_entry();  //合并到多开检测中

/**
 * 包名检验
 * @param pkgVec
 * @return
 */
bool DXAnti_verify_pkg(const string &pkg);

/**
 * 签名检验
 * @param sigVec
 * @return
 */
bool DXAnti_verify_signature(const string &sign);

/**
 * JNI_HOOK Check
 * @return
 */
void DXAnti_detect_jni_hook_entry();


/**
 * no crash
 */
typedef void (*DXAnti_ResultCallback)(int64_t flag, void *args);
void DXAnti_set_monitor_callback(DXAnti_ResultCallback callback);


typedef struct _DXMethod
{
    const char* clsName;
    const char* name;
    const char* sig;
} DXMethod;

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
void DXAnti_detect_jni_hook_add_methods(DXMethod methods[]);

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
