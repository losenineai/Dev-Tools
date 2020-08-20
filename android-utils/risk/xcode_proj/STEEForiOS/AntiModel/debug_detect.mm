//
//  debug_detect.m
//  STEEForiOS
//
//  Created by ccc on 20/03/2018.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#include <sys/sysctl.h>
#include <dlfcn.h>
#import "safe_detect.h"

using namespace std;

DEFINE_CHECK_IMPL(debug) {
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;
    info.kp_proc.p_flag = 0;

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    if(junk != 0 || (info.kp_proc.p_flag & P_TRACED) != 0) {
        call((void *)NULL);
    }

    typedef int (*ptrace_ptr_t)(int _request, pid_t _pid, caddr_t _addr, int _data);
    void *handle = dlopen(0, RTLD_GLOBAL | RTLD_NOW);
    ptrace_ptr_t ptrace_ptr = (ptrace_ptr_t)dlsym(handle, "ptrace");
    ptrace_ptr(31, 0, 0, 0);
    syscall(26,31,0,0,0);

#ifdef __arm__
    asm volatile(
                 "mov r0,#31\n"
                 "mov r1,#0\n"
                 "mov r2,#0\n"
                 "mov r12,#26\n"
                 "svc #80\n"
                 );
#endif
#ifdef __arm64__
    asm volatile(
                 "mov x0,#26\n"
                 "mov x1,#31\n"
                 "mov x2,#0\n"
                 "mov x3,#0\n"
                 "mov x16,#0\n"
                 "svc #128\n"
                 );
#endif
}
