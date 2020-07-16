//
// Created by boyliang on 2017/5/19.
//

#include <stdarg.h>
#include <android/log.h>
#include "DXLog.h"

#define LOG_TAG "DXRisk"
#define LOG_TAG_RLEASE "OpenGLRenderer"


/**
 * 上线日志输出
 */
void LOGR(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG_RLEASE, fmt, args);
    va_end(args);
}

/**
 * 错误日志输出
 */
void LOGE(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, args);
    va_end(args);
}


#ifdef DEBUG
/**
 * 调试日志输出
 */
void LOGD(const char *fmt, ...){
#ifdef INFO_LOG
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, args);
    va_end(args);
#endif
}

#endif

