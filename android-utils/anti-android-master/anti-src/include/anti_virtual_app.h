//
// Created by white on 2018/10/15.
//

#ifndef ANTI_AS_PROJECT_ANTI_VIRTUAL_APP_H
#define ANTI_AS_PROJECT_ANTI_VIRTUAL_APP_H


#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 虚拟app检测
 */
void *DXAnti_detect_virtual_app(void *arg);

int DXAnti_detect_virtual_app_async();

void DXAnti_detect_virtual_app_entry();

#ifdef __cplusplus
}
#endif


#endif //ANTI_AS_PROJECT_ANTI_VIRTUAL_APP_H
