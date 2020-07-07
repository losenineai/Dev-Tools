//
// Created by white on 2018/5/14.
//

#ifndef DX_ANTI_ANDROID_MULTIRUN_H
#define DX_ANTI_ANDROID_MULTIRUN_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 多开检测
 * @return
 */
void *DXAnti_detect_multirun(void *arg);

int DXAnti_detect_multirun_async();

void DXAnti_detect_multirun_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_MULTIRUN_H
