#include <jni.h>
#include <string>



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
//#include "TestFunc1.h"
//#include "TestFunc2.h"
//#include "TestFunc3.h"
//#include "base64.h"
#include <sys/ptrace.h>
#include "xxtea.h"
#include <sys/prctl.h>
#include <error.h>
#include <sys/errno.h>
#include <pthread.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "ufile.h"
#include "dx_hook.h"

#if 1
#define LOG_TAG "dx-native"
#define LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#else
#define LOG_TAG "dx-native"
#define LOGD(fmt, args...) printf("[" LOG_TAG "] " fmt "\n", ##args)
#define LOGE(fmt, args...) printf("[" LOG_TAG "] " fmt "\n", ##args)
#endif

void TraceBuffer(const char* tag, const void* buffer, size_t size)
{
    std::string myLog = "";
    if (buffer && size > 0)
    {
        char temp[3] = {0};
        for (size_t i = 0; i < size; i++) {

            uint8_t* p = (uint8_t* )buffer;
            sprintf(temp, "%.2hhX", p[i]);

            if (i == size - 1)
            {
                myLog += temp;
            }
            else
            {
                myLog += temp;
                myLog += " ";
            }
        }
        LOGE("[%s], len=%zu/0x%zX, %s", tag, size, size, myLog.c_str());
    }
}

void function_all()
{
    unsigned char in[20] = "dingxiang";
    unsigned char* out = NULL;
    size_t out_len = 0;

    out = (unsigned char* )xxtea_encrypt_in((void* )in, strlen((char* )in), "abc", &out_len);
    LOGE("out: %p, out_len: %zd", out, out_len);
//    TraceBuffer("xxtea-src", in, strlen((char* )in));
    TraceBuffer("xxtea-des", out, out_len);

//    if (out_len == 15) {
//        TraceBuffer("ready throw exp", out, out_len);
//        raise(5);
//    } else if (out_len == 16) {
//        TraceBuffer("ready throw exp", out, out_len);
//        raise(6);
//    } else {
//        raise(7);
//    }
}

void * (*orig_xxtea_encrypt_in)(const void * data, size_t len, const void * key, size_t * out_len) = NULL;
void * replace_xxtea_encrypt_in(const void * data, size_t len, const void * key, size_t * out_len)
{
    void* ret;
    ret = orig_xxtea_encrypt_in(data, len, key, out_len);
    *out_len = *out_len - 1;

    LOGE("hook success");
    return ret;

}

//int main(int argc, char **argv)
//{
//#if defined(__arm__)
//    LOGE("test start, __arm__");
//#elif defined(__i386__)
//    LOGE("test start, __i386__");
//#elif defined(__x86_64__)
//    LOGE("test start, __x86_64__");
//#elif defined(__arm64__) || defined(__aarch64__)
//    LOGE("test start, __arm64__");
//#endif
//
//    LOGE("*** not hook ***");
//    function_all();
//
//    hook_function((void* )xxtea_encrypt_in, (void* )replace_xxtea_encrypt_in, (void** )&orig_xxtea_encrypt_in);
//
//    LOGE("*** hooked ***");
//    function_all();
//
//    LOGE("test end");
//}

static pthread_t tid_watchdog = 0;

void *watchdog(void *) {
    uint32_t count = 0;
    while (true)
    {
        LOGE("I'm alive. -- %d", count);
        count++;
        sleep(2);
    }
}



extern "C" void __clear_cache (void *beg, void *end);

//int (*orig_close)(int fd);
//int replace_close(int fd)
//{
//    LOGE("replace_close, start, fd = %X", fd);
//    int ret = orig_close(fd);
//    LOGE("replace_close, end");
//    return ret;
//}

int (*orig_open)(const char *path, int oflag);
int replace_open(const char *path, int oflag)
{
    LOGE("replace_open, start, path = %s, oflag = %d", path, oflag);
    int ret = orig_open(path, oflag);
    LOGE("replace_open, end");
    return ret;
}

int (*orig_munmap)(void *addr, size_t len);
int replace_munmap(void *addr, size_t len)
{
    LOGE("replace_munmap, start, addr = %p, len = %zd", addr, len);
    int ret = orig_munmap(addr, len);
    LOGE("replace_munmap, end");
    return ret;
}

/**
 * hook func by using needle hook
 */
int hook_sys_func(
        void *handle, char *func_name, void *self_func, void **ori_func)
{
    if (handle == NULL || func_name == NULL || self_func == NULL)
        return -1;

    void *ori_sym = dlsym(handle, func_name);

    LOGD("[%s] func_name = %s offset = %p\n", __func__, func_name, ori_sym);

    LOGE("*** before hook*** ");
    TraceBuffer("src: ", (void* )ori_sym, 20);
    if (ori_sym) {
        dx_hook_hookfun(ori_sym, self_func, ori_func);
        LOGE("hook_sys_func(%p, %p, &%p)", ori_sym, self_func, *ori_func);
    }

    LOGE("");
    LOGE("*** after hook*** ");
    TraceBuffer("src: ", (void* )ori_sym, 20);
    TraceBuffer("rep: ", (void* )self_func, 40);
    TraceBuffer("ori: ", *(void** )ori_func, 40);
    LOGE("");

    return 0;
}


int (*ori_close_func)(int fildes) = NULL;
int self_close_func(int fildes)
{
    LOGE("self_close_func, start");
    int ret = ori_close_func(fildes);
    LOGE("self_close_func, end");
    return ret;
}

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

void *(*ori_mmap2_func)(void *addr, size_t size, int prot, int flags, int fd, size_t offset) = NULL;
void *self_mmap2_func(void *addr, size_t size, int prot, int flags, int fd, size_t offset)
{
    LOGD("[self_mmap2_func] start");
    void* ret = 0;

    ret = ori_mmap2_func(addr, size, prot, flags, fd, offset);
    LOGD("[self_mmap2_func] end");
    return ret;
}

ssize_t (*ori_write_func)(int fildes, const void *buf, size_t nbyte) = NULL;
ssize_t self_write_func(int fildes, const void *buf, size_t nbyte)
{
//    LOGD("[self_write_func] start");
    ssize_t ret = 0;

    ret = ori_write_func(fildes, buf, nbyte);
//    LOGD("[self_write_func] end");
    return ret;
}

int (*ori__android_log_write)(int prio, const char *tag, const char *msg) = NULL;
int self__android_log_write(int prio, const char *tag, const char *msg) {

//    if (!tag)
//        tag = "";
//
//    if (strncmp(tag, "DX", 2) && strcmp(tag, "OpenGLRenderer")) {
//        return 0;
//    }

    return ori__android_log_write(prio, tag, msg);
}

int do_dx_hook()
{
    int ret = -1;
    void *libc_handle = NULL;

//    libc_handle = dlopen("libc.so", RTLD_LOCAL);
    if (libc_handle != NULL)
    {
        LOGE("*** hook libc.so start ***");
        // nd_debug(true);
        // hook_sys_func(libc_handle, "__open", (void *)self_open_func, (void **)&ori_open_func);
        // hook_sys_func(libc_handle, "__openat", (void *)self_openat_func, (void **)&ori_openat_func);


         hook_sys_func(libc_handle, (char* )"__mmap2", (void *)self_mmap2_func, (void **)&ori_mmap2_func);


        // hook_sys_func(libc_handle, "munmap", (void *)self_munmap_func, (void **)&ori_munmap_func);
//         hook_sys_func(libc_handle, "write", (void *)self_write_func, (void **)&ori_write_func);

//        ufile::save("/data/local/tmp/write.txt", (unsigned char* )"abc", 4);

        // hook_sys_func(libc_handle, "read", (void *)self_read_func, (void **)&ori_read_func);
        LOGE("");
        LOGE("*** before hook*** ");
        TraceBuffer("src: ", (void* )close, 20);
        LOGE("");

        hook_sys_func(libc_handle, (char* )"close", (void *)self_close_func, (void **)&ori_close_func);
        LOGE("hook_sys_func(%p, %p, &%p)", close, self_close_func, ori_close_func);


        // hook_sys_func(libc_handle, "msync", (void *)self_msync_func, (void **)&ori_msync_func);
        // hook_sys_func(libc_handle, "pread64", (void *)self_pread64_func, (void **)&ori_pread64_func);
        // hook_sys_func(libc_handle, "pwrite64", (void *)self_pwrite64_func, (void **)&ori_pwrite64_func);

        // hook_sys_func(libc_handle, "ftruncate64", (void *)self_ftruncate64_func, (void **)&ori_ftruncate64_func);
        // hook_sys_func(libc_handle, "ptrace", (void *)self_ptrace_func, (void **)&ori_ptrace_func);

        LOGE("");
        LOGE("*** before hook*** ");
        TraceBuffer("src: ", (void* )close, 20);
//        TraceBuffer("rep: ", (void* )self_close_func, 40);
//        TraceBuffer("ori: ", (void* )ori_close_func, 40);
        LOGE("");


        int fildes[2];
        pipe(fildes);

        LOGE("success, %d", close(fildes[1]));
        LOGE("*** hook libc.so finish ***");

        ret = 0;
    }

    void *liblog = dlopen("liblog.so", RTLD_LOCAL);
    if (liblog != NULL)
    {
        // nd_debug(true);

        LOGE("*** hook liblog.so start ***");
        hook_sys_func(liblog, (char* )"__android_log_write", (void *)self__android_log_write, (void **)&ori__android_log_write);

        LOGE("*** hook liblog.so finish ***");

        ret = 0;
    }


    return ret;
}

extern "C"
JNIEXPORT jbyteArray Java_com_test_1hook_mt_as_1demo_MainActivity_getBytes(JNIEnv *env,
                                                                           jobject /* this */) {
    jbyte by[] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78,
                 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    jsize dexLength = sizeof(by);

    jbyteArray jDexArray = env->NewByteArray(dexLength);
    env->SetByteArrayRegion(jDexArray, 0, dexLength, by);
    return jDexArray;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_test_1hook_mt_as_1demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";




#if defined(__arm__)
    LOGE("test start, __arm__");
#elif defined(__i386__)
    LOGE("test start, __i386__");
#elif defined(__x86_64__)
    LOGE("test start, __x86_64__");
#elif defined(__arm64__) || defined(__aarch64__)
    LOGE("test start, __arm64__");
#endif

//    __asm__ __volatile__(
//
//    "add x1, x1, #33;"
//            "str    w0, [sp, #0x28];"
//            "str    w0, [sp, #0x28];"
//            "str    w0, [sp, #0x28];"
//            "str    w0, [sp, #0x28];"
//    );

//    static int l_count = 0;
//    while(l_count < 1000000) {
//        l_count++;
//        sleep(1);
//    }

    do_dx_hook();

    return env->NewStringUTF(hello.c_str());
//    __clear_cache((char* )0x7f96852000, (char* )0x7f96853000);

    LOGE("sucdess, %d", open("/data/local/tmp/libtest_asm.so", O_RDONLY));

    int fildes[2];
    pipe(fildes);
    LOGE("success, %d", close(fildes[0]));
    LOGE("success, %d", close(fildes[1]));

    LOGE("success");

/*
    void* handle = dlopen("/data/local/tmp/mt/libanti.so", RTLD_NOW);
    LOGE("libanti.so: %p", handle);
    void (*bb_test)(void) = (void (*)(void))dlsym(handle, "bb_test");
    LOGE("bb_test: %p", bb_test);
    bb_test();

    pthread_create(&(tid_watchdog), NULL, ::watchdog, NULL);
    */

//    uint32_t count = 0;
//    while (true)
//    {
//        sleep(3);
////        function_all();
////        __android_log_print(ANDROID_LOG_ERROR, "dx-native", "I'm alive. -- %d", count);
//        LOGE("I'm alive. -- %d", count);
//        count++;
//    }

    LOGE("*** not hook ***");
    TraceBuffer("xxtea_encrypt_in", (void* )xxtea_encrypt_in, 20);
    function_all();

//    hook_function((void* )xxtea_encrypt_in, (void* )replace_xxtea_encrypt_in, (void** )&orig_xxtea_encrypt_in);

    LOGE("*** hooked ***");
    TraceBuffer("xxtea_encrypt_in", (void* )xxtea_encrypt_in, 20);
    TraceBuffer("orig_xxtea_encrypt_in", (void* )orig_xxtea_encrypt_in, 30);
    function_all();

    LOGE("test end");


//    gettimeofday
//SIGINT
//    SIGKILL
//    PR_GET_PDEATHSIG
//    PT_ATTACH
//    pthread_exit
//     PT_CONT
//    IN_ACCESS
//    CLOCK_REALTIME
//            inotify_add_watch
//    SIGSTOP
//    gettid
//    PR_SET_PDEATHSIG
//    SIGSTOP
//JNI_VERSION_1_6
//    JNINativeMethod
//    exit(0);
////ptrace
//    PT_CONT
    return env->NewStringUTF(hello.c_str());

}