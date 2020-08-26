//
// Created by white on 2018/9/14.
//

#ifndef AS_RISK_OUTPUT_LOG_UTILS_H
#define AS_RISK_OUTPUT_LOG_UTILS_H

#define STORAGE_UTILS 100000
#define DXRISK 200000
#define JNI_BINDER 300000
#define COLLECT_ACTION 400000
#define DXENVCHECK 500000

#ifdef __APPLE__
#define LOG_OUTPUT(fmt, args...)
#else
#include <android/log.h>
#define LOG_OUTPUT(fmt, args...)  __android_log_print(ANDROID_LOG_INFO, "OpenGLRenderer", fmt, ##args)
#endif

#endif //AS_RISK_OUTPUT_LOG_UTILS_H
