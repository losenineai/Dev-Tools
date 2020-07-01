//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_ANTI_LOG_H
#define DX_ANTI_ANDROID_ANTI_LOG_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

void *DXAnti_detect_log(void *arg);

int DXAnti_detect_log_async();

void DXAnti_detect_log_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_LOG_H
