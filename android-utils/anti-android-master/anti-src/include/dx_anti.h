//
// Created by white on 2018/5/22.
//

#ifndef ANDROID_DX_ANTI_H
#define ANDROID_DX_ANTI_H

#include <jni.h>
#include <stdint.h>
#include "anti_config.h"





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
#define DXANTI_FLAG_VIRTUAL_APP (1 << 10)
/**
 * 无守护进程的反调试检测
 * 启动必须： DXANTI_FLAG_DEBUG | DXANTI_FLAG_DEBUG_EXT_NO_DAEMON
 */
#define DXANTI_FLAG_DEBUG_EXT_NO_DAEMON (1 << 11)
//#define DXANTI_FLAG_PROXY 1 << 8


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

#ifdef __cplusplus
}
#endif

#endif //ANDROID_DX_ANTI_H
