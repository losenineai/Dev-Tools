//
// Created by white on 2018/11/28.
//

#ifndef ANTI_AS_PROJECT_ANTI_DEBUG_INNER_H
#define ANTI_AS_PROJECT_ANTI_DEBUG_INNER_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 反调试，单线程检测
 * @return
 */
void *DXAnti_detect_debug_inner(void *arg);

int DXAnti_detect_debug_inner_async(pthread_t &tid);

void DXAnti_detect_debug_inner_entry();


#ifdef __cplusplus
}
#endif


#endif //ANTI_AS_PROJECT_ANTI_DEBUG_INNER_H
