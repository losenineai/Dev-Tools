#ifndef ALI_JNI_LOG_H_
#define ALI_JNI_LOG_H_

#define TRACE_ENABLE 1
#define DEBUG_ENABLE 1

#if ANDROID
#include <android/log.h>
#define MOCK_INFO(args...) __android_log_print(ANDROID_LOG_INFO, "MOCK", ##args)
#define MOCK_ERROR(args...) __android_log_print(ANDROID_LOG_ERROR, "MOCK", ##args)

#if DEBUG_ENABLE
#define MOCK_DEBUG(args...) __android_log_print(ANDROID_LOG_DEBUG, "MOCK", ##args)
#else
#define MOCK_DEBUG(args...) (0)
#endif

#if TRACE_ENABLE
#define LOGT(args...) __android_log_print(ANDROID_LOG_DEBUG, "MOCK", ##args)
#else
#define LOGT(args...) (0)
#endif
#else
#include <stdio.h>
#define MOCK_INFO(fmt, args...) printf("I: %-20s| " fmt "\n", __FUNCTION__, ##args)
#define MOCK_ERROR(fmt, args...) printf("E: %-20s| " fmt "\n", __FUNCTION__, ##args)

#if DEBUG_ENABLE
#define MOCK_DEBUG(fmt, args...) printf("D: %-20s| " fmt "\n", __FUNCTION__, ##args)
#else
#define MOCK_DEBUG(args...) (0)
#endif

#if TRACE_ENABLE
#define MOCK_TRACE(fmt, args...) printf("T: %-20s| " fmt "\n", __FUNCTION__, ##args)
#else
#define MOCK_TRACE(args...) (0)
#endif
#endif

#endif
