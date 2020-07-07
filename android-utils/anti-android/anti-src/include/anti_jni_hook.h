//
//  DXJavaHookChecker.h
//  JavaHookChecker
//
//  Created by mt on 2017/10/18.
//  Copyright © 2017年 mt. All rights reserved.
//

#ifndef DXJavaHookChecker_h
#define DXJavaHookChecker_h

#include <stdio.h>
#include <jni.h>
#include "anti_config.h"


#ifdef __cplusplus
extern "C" {
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




void DXAnti_detect_jni_hook_entry();

int DXAnti_detect_jni_hook_async(pthread_t &tid);

void *DXAnti_detect_jni_hook(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* DXJavaHookChecker_h */

