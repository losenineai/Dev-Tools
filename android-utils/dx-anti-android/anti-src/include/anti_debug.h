//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_ANTI_DEBUG_H
#define DX_ANTI_ANDROID_ANTI_DEBUG_H


#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 反调试检测
 * @return
 */
void *DXAnti_detect_debug(void *arg);

int DXAnti_detect_debug_async();

void DXAnti_detect_debug_entry();


#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_DEBUG_H
