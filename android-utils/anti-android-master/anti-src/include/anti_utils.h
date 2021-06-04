//
// Created by blue on 2018/7/10.
//

#ifndef DX_ANTI_ANDROID_ANTI_UTILS_H
#define DX_ANTI_ANDROID_ANTI_UTILS_H

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <android/log.h>

#include "anti_config.h"

#define LOG_TAG "DXAnti"
#define LOGE(fmt, args...) // __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#define LOG_CRASH(fmt, args...)  __android_log_print(ANDROID_LOG_ERROR, "OpenGLRenderer", fmt, ##args)
#define LOG_WARNING(fmt, args...)  __android_log_print(ANDROID_LOG_WARN, "OpenGLRenderer", fmt, ##args)
#define LOG_LOG(fmt, args...)  __android_log_print(ANDROID_LOG_ERROR, "DXAnti", fmt, ##args)
#define LOGLOCATION LOGE("%s (%d) - <%s>",__FILE__,__LINE__,__FUNCTION__);

#define dx_pthread_create(ret, thread, attr, start_routine, arg)                \
    for (int pcex = 0; pcex < 32; ++pcex)                                       \
    {                                                                           \
        ret = pthread_create(thread, attr, start_routine, arg);                 \
        if (0 == ret) {                                                         \
             break;                                                             \
        }                                                                       \
        sleep(1);                                                               \
    }


extern int dx_set_errno(int n);

extern int dx_kill(pid_t pid, int sig);

extern int bb_gettimeofday(struct timeval * tp, struct timezone * tzp);

extern int bb_timer_delete(timer_t timerid);

extern void* malloc_wraper(void* pval, size_t len);

extern bool dx_hook2(const char* libpath, const char *symbol, void *replace, void **origin);

bool dx_select(int fd, int timeOut, bool read);

const char *dx_get_sys_lib_path(const char *name);

#endif //DX_ANTI_ANDROID_ANTI_UTILS_H
