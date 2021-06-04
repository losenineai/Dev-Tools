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

#define DXAnti_verify_pkg                                               go0O9o10LLgiOIIILilL
#define DXAnti_verify_signature                                         gL1LIL1lo0iil0Il9Oo0
#define DXAnti_set_monitor_callback                                     O0i0Og9l0OOl0LlLiO11


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
#define DXANTI_FLAG_VIRTUAL_APP (1 << 10)
/**
 * 无守护进程的反调试检测
 * 启动必须： DXANTI_FLAG_DEBUG | DXANTI_FLAG_DEBUG_EXT_NO_DAEMON
 */
#define DXANTI_FLAG_DEBUG_EXT_NO_DAEMON (1 << 11)



/**
 * anti函数返回状态码
 */
typedef enum _DXAntiRetStatus{
    INIT_SUCCESS = 0,
    INIT_FAIL_ALREADY_INIT = 1,
    INIT_FAIL_PARAMS_NULL = 2
}DXAntiRetStatus;


/**
 * jni 环境初始化
 * 注意：Anti功能使用前必须调用
 * @param vm
 * @param jContext jni Context实例
 */
DXAntiRetStatus DXAnti_monitor_init(JavaVM *vm, jobject jContext);


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
void DXAnti_detect_virtual_app_entry();

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
 * no crash
 */
typedef void (*DXAnti_ResultCallback)(int64_t flag, void *args);
void DXAnti_set_monitor_callback(DXAnti_ResultCallback callback);


#ifdef __cplusplus
}
#endif

#endif //ANDROID_DX_ANTI_H
