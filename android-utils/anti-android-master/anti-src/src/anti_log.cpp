//
// Created by white on 2018/5/15.
//

#include <cstring>
#include <dlfcn.h>
#include <string.h>
#include "utils_anti_log.h"
#include "anti_log.h"
#include "dx_anti_stee.h"
#include "dx_libc_dlfcn.h"
#include "anti_utils.h"

/**
 * 通过检测ro.build.xxx
 * @return
 */

int (*dx_android_log_buf_write)(int bufID, int prio, const char *tag, const char *msg) = NULL;
int (*dx_android_log_write)(int prio, const char *tag, const char *msg) = NULL;

STEE
int anti_log_replace_buf_write(int bufID, int prio, const char *tag, const char *msg) {

    if (!tag)
        tag = "";

    if (strncmp(tag, "DX", 2) == 0 || strcmp(tag, "OpenGLRenderer") == 0) {
        return dx_android_log_buf_write(bufID, prio, tag, msg);
    }

    if(prio == ANDROID_LOG_ERROR || prio == ANDROID_LOG_FATAL){
        dx_android_log_buf_write(bufID, prio, tag, msg);
    }

    return 0;
}

STEE
int anti_log_replace_write(int prio, const char *tag, const char *msg) {

    if (!tag)
        tag = "";

    if (strncmp(tag, "DX", 2) == 0 || strcmp(tag, "OpenGLRenderer") == 0) {
        return dx_android_log_write(prio, tag, msg);
    }

    if(prio == ANDROID_LOG_ERROR || prio == ANDROID_LOG_FATAL){
        return dx_android_log_write(prio, tag, msg);
    }

    return 0;
}

void *DXAnti_detect_log(void *args){
    const char* libcutilspath = dx_get_sys_lib_path("libcutils.so");
    const char* liblogPath = dx_get_sys_lib_path("liblog.so");

    dx_hook2(libcutilspath, "__android_log_buf_write", (void* )anti_log_replace_buf_write, (void **)&dx_android_log_buf_write);
    dx_hook2(libcutilspath, "__android_log_write", (void* )anti_log_replace_write, (void **)&dx_android_log_write);

    dx_hook2(liblogPath, "__android_log_buf_write", (void* )anti_log_replace_buf_write, (void **)&dx_android_log_buf_write);
    dx_hook2(liblogPath, "__android_log_write", (void* )anti_log_replace_write, (void **)&dx_android_log_write);

    return NULL;
}

STEE
int DXAnti_detect_log_async() {

    pthread_t newthread;
    int ret = -1;

    dx_pthread_create(ret, &newthread, NULL, DXAnti_detect_log, NULL);

    return ret;
}



STEE
void DXAnti_detect_log_entry() {
    DXAnti_detect_log_async();
}
