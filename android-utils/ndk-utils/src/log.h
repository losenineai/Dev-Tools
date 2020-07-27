//
// Created by white on 2020/7/25.
//

#ifndef LOG_H
#define LOG_H

#include <android/log.h>

#define DEBUG

#define LOG_TAG "ndk-utils"

#ifdef DEBUG
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATA, LOG_TAG, __VA_ARGS__))

#else
#define LOGD(...)
#define LOGV(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATA, LOG_TAG, __VA_ARGS__))
#endif

#endif //LOG_H
