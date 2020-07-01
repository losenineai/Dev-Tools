//
// Created by white on 2017/5/19.
//

#include <stdarg.h>
#include <android/log.h>
#include "log.h"


/**
 * 错误日志输出
 */
void LOGE(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, TAG, fmt, args);
    va_end(args);
}

/**
 * 普通日志输出
 */
void LOGI(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, args);
    va_end(args);
}

