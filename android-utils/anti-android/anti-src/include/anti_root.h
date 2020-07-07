//
// Created by white on 2018/5/14.
//

#ifndef DX_ANTI_ANDROID_ANTI_ROOT_H
#define DX_ANTI_ANDROID_ANTI_ROOT_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * root 检测
 * @return
 */
void *DXAnti_detect_root(void *arg);

int DXAnti_detect_root_async();

void DXAnti_detect_root_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_ROOT_H
