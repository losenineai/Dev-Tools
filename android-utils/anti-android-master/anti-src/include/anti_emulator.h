//
// Created by blue on 2018/7/4.
//

#ifndef ANDROID_ANTI_EMULATOR_H
#define ANDROID_ANTI_EMULATOR_H

#include "anti_config.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 调试模拟器
 * @return
 */
void *DXAnti_detect_emulator(void *arg);

int DXAnti_detect_emulator_async();

void DXAnti_detect_emulator_entry();

#ifdef __cplusplus
}
#endif

#endif //ANDROID_ANTI_EMULATOR_H
