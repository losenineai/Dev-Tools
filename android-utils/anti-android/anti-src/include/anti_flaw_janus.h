//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_ANTI_FLAW_JANUS_H
#define DX_ANTI_ANDROID_ANTI_FLAW_JANUS_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * apk 非法检测
 * @return
 */
void *DXAnti_detect_flaw_janus(void *arg);

int DXAnti_detect_flaw_janus_async();

void DXAnti_detect_flaw_janus_entry();

#ifdef __cplusplus
}
#endif

#endif //DX_ANTI_ANDROID_ANTI_FLAW_JANUS_H
