//
// Created by blue on 2018/7/10.
//

#include <jni.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <jni_utils.h>
#include <src/utils/include/comm_utils.h>
#include <thread>
#include <sys/syscall.h>
#include "anti_utils.h"
#include "dx_libc.h"
#include "dx_hook.h"
#include "dx_anti_stee.h"

int dx_set_errno(int n) {
    errno = n;
    return -1;
}

const char *dx_get_sys_arm_lib_path(const char *name);
const char *dx_get_sys_lib_path(const char *name);

STEE
void dx_go_hell() {

    uint64_t sleep_time = anti_util_createRandom(10);
    anti_util_sleepMS(sleep_time);

    uint32_t case_index = (uint32_t) anti_util_createRandom(6);

    // 递归线程调用
    function<void()> run_loop_by_thread1 = [&]() {
        for (int i = 0; i < 10; i++) {
            thread dd(run_loop_by_thread1);
            if (dd.joinable()) {
                dd.detach();
            }
        }

        anti_util_sleepSEC(anti_util_createRandom(10));
    };

    function<int(int a, int b)> run_loop_by_int;
    // 递归调用
    run_loop_by_int = [=](int a, int b) {
        return a + b + run_loop_by_int(a + 1, b + 1);
    };

    // 递归线程调用
    function<void()> run_loop_by_thread2 = [&]() {
        for (int i = 0; i < 100; i++) {
            thread dd(run_loop_by_thread2);
            if (dd.joinable()) {
                dd.detach();
            }
        }
        run_loop_by_int(1, 1);

    };

    // 各种死法
    switch (case_index) {
        case 0: {
            while (true) {
                uint8_t *p = new uint8_t[anti_util_createRandom(10) * 256];
                *p = anti_util_createRandom(256);
                anti_util_sleepMS(anti_util_createRandom(10));

                // just make compiler happy!!
                if ((*p % 2) == 0) {
                    abort();
                }
            }
            break;
        }

        case 1: {
            thread dd(run_loop_by_thread1);
            if (dd.joinable()) {
                dd.detach();
            }
            break;
        }

        case 2: {
            thread dd(run_loop_by_thread2);
            if (dd.joinable()) {
                dd.detach();
            }
            break;
        }

        case 3: {
            auto abort_impl = [=]() {
                // just for fun
                dx_go_hell();
                exit(-1);
            };

            thread td(abort_impl);
            if (td.joinable()) {
                td.detach();
            }

            break;
        }

        case 4: {
            uint8_t *a_mem_to_call = new uint8_t[1024];

            memset(a_mem_to_call, anti_util_createRandom(10), sizeof(1024));
            void (*callable_ptr)(void *) = (void (*)(void *)) (uintptr_t) a_mem_to_call;
            callable_ptr((void *) (intptr_t) 2488);

            // just for fun
            dx_go_hell();
            break;
        }

        case 5: {
            // 野指针
            while (true) {
                uint16_t *p = new uint16_t[anti_util_createRandom(5)];
                p = (p + anti_util_createRandom(0xFFFF));
                anti_info_log("death p %d", p);
                if (*p > 0) {
                    anti_info_log("death @@@@");
                    abort();
                }
            }
        }

        default:
            dx_go_hell();
            break;
    }
}

STEE
int dx_kill(pid_t pid, int sig) {

    // int result; // r0@1
    // result = linux_eabi_syscall(__NR_kill, pid, sig);
    // if (result < -0x1000) {
    //     result = dx_set_errno(-result);
    // }
    // return result;

    // by white, for chushou bugly crash
//    if(pid == getpid()) {
//        dx_go_hell();
//        return 1;
//    } else {
//        return (int)syscall(__NR_kill, pid, sig);
//    }

    return (int)syscall(__NR_kill, pid, sig);
}

STEE
int bb_gettimeofday(struct timeval * tp, struct timezone * tzp) {
    return gettimeofday(tp, tzp);
}

STEE
int bb_timer_delete(timer_t timerid) {
    return timer_delete(timerid);
}

STEE
void* malloc_wraper(void* pval, size_t len) {
    void** p = (void** )malloc(sizeof(pid_t));
    memcpy(p, pval, len);
    return (void* )p;
}

STEE
bool dx_hook2(const char* libpath, const char *symbol, void *replace, void **origin)
{   
    anti_info_log("dx_hook2 libpath:%s symbol:%s", libpath, symbol);
    void* dxhandle = NULL;
    void* dxsym = NULL;
    // dxhandle = dlopen(libpath, RTLD_NOW);
    dxhandle = dx_dlopen(libpath, RTLD_NOW);

    anti_info_log("dxhandle addr %p", dxhandle);
    if (dxhandle == NULL) {
        return false;
    } else {
        anti_info_log("dxsym addr 1");
        // dxsym = dlsym(dxhandle, symbol);
        dxsym = dx_dlsym(dxhandle, symbol);

        anti_info_log("dxsym addr 22 %p", dxsym);
        dx_dlclose(dxhandle);
        // dlclose(dxhandle);
        if (dxsym == NULL) {
            return false;
        }

        dx_hook_hookfun(dxsym, replace, origin);
        anti_info_log("dx_hook_hookfun");
    }
    return true;
}

STEE
bool dx_select(int fd, int timeOut, bool read)
{
    int maxFd = 0;
    if (fd >= maxFd)
        maxFd = fd + 1;

    fd_set fdset;
    timeval tv;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    timeOut = timeOut > 1000 ? 1000 : timeOut;
    tv.tv_sec  = 0;
    tv.tv_usec = timeOut;

    int ret = 0;
    if (read)
        ret = select(maxFd, &fdset, nullptr, nullptr, &tv);
    else
        ret = select(maxFd, nullptr, &fdset, nullptr, &tv);

    //ret == -1, error handled by u;
    //ret == 0, timeout hanled by u;
    if (ret <= 0)
        return false;
    else if (FD_ISSET(fd, &fdset))
        return true;
    return false;
}

STEE
const char *dx_get_sys_lib_path(const char *name){
    char *path = NULL;
    const char *prePath = (sizeof(void*) == 8) ? "/system/lib64/" : "/system/lib/";

    int len = dx_strlen(prePath) + strlen(name) + 1;
    path = (char *)malloc(sizeof(char) * len);
    dx_strcpy(path, prePath);
    dx_strcat(path, name);

    return path;
}
