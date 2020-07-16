//
// Created by boyliang on 2017/5/19.
//

#ifndef DEVICEFINGERPRINT_LOG_H
#define DEVICEFINGERPRINT_LOG_H

#include <stdio.h>
#include "core_prefix.h"

#ifdef __cplusplus
extern "C" {
#endif


#define TAG_STORAGE_UTILS           100000
#define TAG_DXRISK                  200000
#define TAG_JNI_BINDER              300000
#define TAG_COLLECT_ACTION          400000
#define TAG_DXENVCHECK              500000


/**
 * 上线日志输出
 */
void LOGR(const char *fmt, ...);

/**
 * 错误日志输出
 */
void LOGE(const char *fmt, ...);


#ifdef DEBUG
    /**
     * 开发日志输出
     */
    void LOGD(const char *fmt, ...);
#else
#define LOGD(...)
#endif


#ifdef __cplusplus
}
#endif

#endif //DEVICEFINGERPRINT_LOG_H
