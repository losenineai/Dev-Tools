//
// Created by blue on 2018/7/11.
//

#ifndef DX_ANTI_ANDROID_ANTI_THREAD_H
#define DX_ANTI_ANDROID_ANTI_THREAD_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

void *DXAnti_detect_thread(void *arg);

int DXAnti_detect_thread_async(pid_t pid);

void DXAnti_detect_thread_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_THREAD_H
