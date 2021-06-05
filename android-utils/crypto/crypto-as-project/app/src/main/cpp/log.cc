//
// Created by boyliang on 2017/5/19.
//

#include <stdarg.h>
#include <android/log.h>
#include "log.h"

#define LOG_TAG "DXCryptoTest"

/**
 * 错误日志输出
 */
void err_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, args);
    va_end(args);
}

/**
 * 普通日志输出
 */
void info_log(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, args);
    va_end(args);
}

#ifdef DEBUG



/**
 * 流程日志，非敏感日志
 */
void flow_log(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_VERBOSE, LOG_TAG, fmt, args);
    va_end(args);
}
#endif

