//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_ANTI_MEMDUMP_H
#define DX_ANTI_ANDROID_ANTI_MEMDUMP_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 内存 dump 检测
 * @return
 */

void *DXAnti_detect_memdump(void *arg);

int DXAnti_detect_memdump_async(pthread_t &tid, pid_t pid);

void DXAnti_detect_memdump_entry();

#ifdef __cplusplus
}
#endif


#endif //DX_ANTI_ANDROID_ANTI_MEMDUMP_H
