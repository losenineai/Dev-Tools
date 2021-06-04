//
// Created by boyliang on 2017/5/19.
//

#include <android/log.h>
#include "utils_anti_log.h"

#define ANTI_LOG_TAG "DXAnti"


/**
 * 错误日志输出
 */
void anti_err_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, ANTI_LOG_TAG, fmt, args);
    va_end(args);
}


#ifdef ANTI_DEBUG
/**
 * 普通日志输出
 */
void anti_info_log(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, ANTI_LOG_TAG, fmt, args);
    va_end(args);
}


/**
 * 流程日志，非敏感日志
 */
void anti_flow_log(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(ANDROID_LOG_VERBOSE, ANTI_LOG_TAG, fmt, args);
    va_end(args);
}
#endif

