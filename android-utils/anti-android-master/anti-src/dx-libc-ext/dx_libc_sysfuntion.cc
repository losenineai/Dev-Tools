//
// Created by amlloc on 18-1-30.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/stat.h>
// #include <dx_utils_errno.h>
#include <unistd.h>
#include <cstring>
#include "dx_libc_local.h"
/*
 * 只使用汇编语言实现了x86平台的syscall()，
 * 其他平台未使用汇编实现，而是利用syscall()函数来替代
 */

inline long dxsyscall(long num, long a1, void *buff, long a3, long a4, long a5, long a6) {
//    long dxerrno;
//    __asm__  volatile(
//
//            "svc     $0\n"
//            :"=r"(dxerrno)
////          :"r0" (num)
//            :"r" (num), "r"(a1), "r"(buff), "r"(a3)
//            :"r0", "r1", "r2", "r3"
//    );
//
//    return dxerrno;


#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
    return syscall(num, a1, buff, a3, a4, a5, a6);
#else
#define ABI "armeabi-v7a"

    return syscall(num, a1, buff, a3, a4, a5, a6);

#endif
#else
#define ABI "armeabi"

//下面的代码还无法跑通，有待研究
//    long dxerrno;
//    __asm__  volatile(
//    "mov %%r7, %%r1\n"
//            "push {%%r0}\n"
//            "mov %%r0, %%r2\n"
//            "mov %%r1, %%r3\n"
//            "pop {%%r2}\n"
//            "push {%%r3}\n"
//            "push {%%r4}\n"
//            "push {%%r5}\n"
//            "push {%%r6}\n"
//            "svc     $0\n"
//    :"=r"(dxerrno)
////    :"r0" (num), "r1"(a1), "r2"(buff), "r3"(a3), "r4"(a4), "r5"(a5)
//    :"r" (num), "r"(a1), "r"(buff), "r"(a3), "r"(a4)
//    );
//
//    if (dxerrno >= -4095 && dxerrno <= -1) {
//        errno = -dxerrno;
//    }
//
//    return dxerrno;

//使用syscall()的方式进行
    return syscall(num, a1, buff, a3, a4, a5, a6);

#endif
#elif defined(__i386__)
#define ABI "x86"
    long dxerrno;
    __asm__ volatile(

    "int     $0x80;"
    :"=r"(dxerrno)
    :"a" (num), "b"(a1), "c"(buff), "d"(a3), "S"(a4), "D"(a5)
    );

    if (dxerrno >= -4095 && dxerrno <= -1) {
        errno = -dxerrno;
    }

    return dxerrno;

#elif defined(__mips__)
#define ABI "mips"

    return syscall(num, a1, buff, a3, a4, a5, a6);

#else
#define ABI "unknown"
    return syscall(num, a1, buff, a3, a4, a5, a6);
#endif

}

inline long dxsyscall2(long num, long a1, long a2, long a3, long a4, long a5, long a6) {

#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)

#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"

    return syscall(num, a1, a2, a3, a4, a5, a6);


#else
#define ABI "armeabi-v7a"

    return syscall(num, a1, a2, a3, a4, a5, a6);


#endif


#else
#define ABI "armeabi"

//下面的代码还无法跑通，有待研究
//    long dxerrno;
//    __asm__  volatile(
//
////    "mov %%r12, %%sp\n"
//            "mov %%r7, %%r1\n"
//            "push {%%r0}\n"
//            "mov %%r0, %%r2\n"
//            "mov %%r1, %%r3\n"
//            "pop {%%r2}\n"
//            "pop {%%r3}\n"
//            "pop {%%r4}\n"
//            "pop {%%r5}\n"
//            "pop {%%r6}\n"
//            "svc     $0\n"
//    :"=r"(dxerrno)
////    :"r0" (num), "r1"(a1), "r2"(buff), "r3"(a3), "r4"(a4), "r5"(a5)
//    :"r" (num), "r"(a1), "r"(a2), "r"(a3), "r"(a4)
//    );
//
//    if (dxerrno >= -4095 && dxerrno <= -1) {
//        errno = -dxerrno;
//    }
//
//    return dxerrno;


//以下使用syscall()的方式

    return syscall(num, a1, a2, a3, a4, a5, a6);

#endif

#elif defined(__i386__)
#define ABI "x86"
    long dxerrno;
    __asm__ volatile(

    "int     $0x80;"
    :"=r"(dxerrno)
    :"a" (num), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5)
    );

    if (dxerrno >= -4095 && dxerrno <= -1) {
        errno = -dxerrno;
    }

    return dxerrno;


#elif defined(__mips__)
#define ABI "mips"

    return syscall(num, a1, a2, a3, a4, a5, a6);

#else
#define ABI "unknown"

    return -1;

#endif
}

inline long dxsyscall3(long num, long a1, void *buff, void *a3, long a4, long a5, long a6) {
//    long dxerrno;
//    __asm__  volatile(
//
//            "svc     $0\n"
//            :"=r"(dxerrno)
////          :"r0" (num)
//            :"r" (num), "r"(a1), "r"(buff), "r"(a3)
//            :"r0", "r1", "r2", "r3"
//    );
//
//    return dxerrno;


#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
    return syscall(num, a1, buff, a3, a4, a5, a6);
#else
#define ABI "armeabi-v7a"

    return syscall(num, a1, buff, a3, a4, a5, a6);

#endif
#else
#define ABI "armeabi"

//下面的代码还无法跑通，有待研究
//    long dxerrno;
//    __asm__  volatile(
//    "mov %%r7, %%r1\n"
//            "push {%%r0}\n"
//            "mov %%r0, %%r2\n"
//            "mov %%r1, %%r3\n"
//            "pop {%%r2}\n"
//            "push {%%r3}\n"
//            "push {%%r4}\n"
//            "push {%%r5}\n"
//            "push {%%r6}\n"
//            "svc     $0\n"
//    :"=r"(dxerrno)
////    :"r0" (num), "r1"(a1), "r2"(buff), "r3"(a3), "r4"(a4), "r5"(a5)
//    :"r" (num), "r"(a1), "r"(buff), "r"(a3), "r"(a4)
//    );
//
//    if (dxerrno >= -4095 && dxerrno <= -1) {
//        errno = -dxerrno;
//    }
//
//    return dxerrno;

//使用syscall()的方式进行
    return syscall(num, a1, buff, a3, a4, a5, a6);

#endif
#elif defined(__i386__)
    #define ABI "x86"
    long dxerrno;
    __asm__ volatile(

    "int     $0x80;"
    :"=r"(dxerrno)
    :"a" (num), "b"(a1), "c"(buff), "d"(a3), "S"(a4), "D"(a5)
    );

    if (dxerrno >= -4095 && dxerrno <= -1) {
        errno = -dxerrno;
    }

    return dxerrno;

#elif defined(__mips__)
    #define ABI "mips"

    return syscall(num, a1, buff, a3, a4, a5, a6);

#else
#define ABI "unknown"
    return syscall(num, a1, buff, a3, a4, a5, a6);
#endif

}
int dxsys_open(const char *name, int oflag, mode_t mode, int dirfd) {

    int file_fd;

    file_fd = (int) dxsyscall(__NR_openat, dirfd, (char *) name, oflag, mode);
    return file_fd;

}

ssize_t dxsys_read(int fd, void *buff, size_t size) {
    return (ssize_t) dxsyscall(__NR_read, fd, buff, size);
}

//
ssize_t dxsys_write(int fd, const void *buff, size_t size) {
    return (ssize_t) dxsyscall(__NR_write, fd, (char *) buff, size);
}

off_t dxsys_lseek(int fd, off_t offset, int whence) {
    return (off_t) dxsyscall2(__NR_lseek, fd, offset, whence);
}

//
int dxsys_close(int fd) {
    return (int) dxsyscall(__NR_close, fd);
}

pid_t dxsys_getpid(){
    return (pid_t)dxsyscall(__NR_getpid);
}

int dxsys_sigaction(int __signal, const struct sigaction* __new_action, struct sigaction* __old_action){
    return (int)dxsyscall3(134, __signal, (void *) __new_action, (void *)__old_action);
}

