//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_ANTI_INJECT_H
#define DX_ANTI_ANDROID_ANTI_INJECT_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

void *DXAnti_detect_inject(void *arg);

int DXAnti_detect_inject_async();

void DXAnti_detect_inject_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_INJECT_H
