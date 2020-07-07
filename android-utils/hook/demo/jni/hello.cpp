#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "TestFunc1.h"
#include "TestFunc2.h"
#include "TestFunc3.h"
#include "base64.h"
#include "xxtea.h"
#include "dx_hook.h"
#include <dlfcn.h>
#include <map>
#include <sys/mman.h>
#include "log.h"

extern "C" void __clear_cache (void *beg, void *end);

static uintptr_t l_image = 0;

std::map<std::string, size_t> moduleBaseMap;

size_t get_module_base(const std::string& moduleName = "", pid_t pid = -1, bool cache = true)
{
    char buff[512] = {0};
    int libNameLen = moduleName.length();
    if (libNameLen == 0)
        return 0;

    if (moduleBaseMap.find(moduleName) != moduleBaseMap.end())
        return moduleBaseMap[moduleName];

    char filename[128];
    if (pid < 0)
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    else
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    FILE* file = fopen(filename, "rt");
    if (file == nullptr)
        return 0;

    char flags[4] = {0};
    size_t addr = 0;
    size_t start, end, offset;
    while (fgets(buff, sizeof buff, file) != nullptr)
    {
        int len = (int)strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';
        if (len <= libNameLen || memcmp(buff + len - libNameLen, moduleName.c_str(), libNameLen))
            continue;
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        if (flags[0] != 'r' || flags[2] != 'x')
            continue;
        addr = start - offset;
        break;
    }
    fclose(file);

    if (cache)
        moduleBaseMap[moduleName] = addr;

    return addr;
}



void trace_maps_file()
{
    char buff[512] = {0};

    FILE* file = fopen("/proc/self/maps", "rt");
    if (file == nullptr)
        return;

    char flags[4] = {0};
    size_t addr = 0;
    size_t start, end, offset;
    while (fgets(buff, sizeof buff, file) != nullptr)
    {
        if (buff[strlen(buff) - 1] == '\n') {
            buff[strlen(buff) - 1] = '\0';
        }
        LOGD("line: %s", buff);
    }
    fclose(file);
}

void function_all()
{
    unsigned char in[20] = "dingxiang";
    unsigned char* out = NULL;
    size_t out_len = 0;

    out = (unsigned char* )xxtea_encrypt_in((void* )in, strlen((char* )in), "abc", &out_len);
    LOGD("out: %p, out_len: %zd", out, out_len);
    TraceBuffer("xxtea-src", in, strlen((char* )in));
    TraceBuffer("xxtea-des", out, out_len);
}

void * (*orig_xxtea_encrypt_in)(const void * data, size_t len, const void * key, size_t * out_len) = NULL;
void * replace_xxtea_encrypt_in(const void * data, size_t len, const void * key, size_t * out_len)
{
    void* ret;
    ret = orig_xxtea_encrypt_in(data, len, key, out_len);
    LOGD("hook success");
    return ret;
}

/**
 * hook func by using needle hook
 */
int hook_sys_func(void *handle, char *func_name, void *self_func, void **ori_func)
{
    if (handle == NULL || func_name == NULL || self_func == NULL)
        return -1;

    void *ori_sym = dlsym(handle, func_name);

    LOGD("[%s] func_name = %s offset = %p\n", __func__, func_name, ori_sym);

    LOGE("*** before hook*** ");
    TraceBuffer("src: ", (void* )ori_sym, 16);
    if (ori_sym) {
        dx_hook_hookfun(ori_sym, self_func, ori_func);
        LOGE("hook_sys_func(%p, %p, &%p)", ori_sym, self_func, *ori_func);
    }

    LOGE("");
    LOGE("*** after hook*** ");
    TraceBuffer("src: ", (void* )ori_sym, 16);
    TraceBuffer("rep: ", (void* )self_func, 40);
    TraceBuffer("ori: ", *(void** )ori_func, 50);
    LOGE("");
    return 0;
}

int (*ori_close_func)(int fildes) = NULL;
int self_close_func(int fildes)
{
    return ori_close_func(fildes);
}

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
    // LOGD("[self_write_func] start");
    ssize_t ret = 0;

    ret = ori_write_func(fildes, buf, nbyte);
    // LOGD("[self_write_func] end");
    return ret;
}

void* dx_utils_hook_memcpy(void* src, void* des, size_t size)
{
    uint8_t* s = (uint8_t* )src;
    uint8_t* d = (uint8_t* )des;

    for (size_t i = 0; i < size; ++i, s++, d++)
    {
        *s = *d;
    }
    return s;
}

bool nd_utils_patch_code(void* addr, void* data, size_t len) {
    if (len == 0) {
        return false;
    }

    size_t page = sysconf(_SC_PAGESIZE);
    uintptr_t base = (uintptr_t )addr / page * page;
    size_t width = (((uintptr_t )addr + len - 1) / page + 1) * page - base;

    // if (addr != data) {
        LOGE("repair mprotect");
        if (mprotect((void* )base, width, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
            // MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);

            LOGE("repair mprotect error");
            return false;
        }

        dx_utils_hook_memcpy((void* )addr, data, len);

        LOGE("restore mprotect(%p, 0x%X)", (void* )base, width);

        if (mprotect((void* )base, width, PROT_READ | PROT_EXEC) == -1) {
            // MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);
            LOGE("restore mprotect error");
        }
    // }

    __clear_cache((char* )base, (char* )base + width);

    return true;
}

int (*src__android_log_write)(int prio, const char *tag, const char *msg) = NULL;
int (*ori__android_log_write)(int prio, const char *tag, const char *msg) = NULL;
int self__android_log_write(int prio, const char *tag, const char *msg) {
    int ret = 0;
//    if (!tag)
//        tag = "";
//
//    if (strncmp(tag, "DX", 2) && strcmp(tag, "OpenGLRenderer")) {
//        return 0;
//    }
    LOGE("*** self__android_log_write, start");

    // trace_maps_file();
    LOGE("*** self__android_log_write, 222");
    LOGE("*** self__android_log_write, prio: %d, tag: %s, msg: %s", prio, tag, msg);
    LOGE("*** self__android_log_write, 333");
    LOGE("*** self__android_log_write, ori__android_log_write: %p", ori__android_log_write);
    TraceBuffer("self__android_log_write", (void* )ori__android_log_write, 40);
    LOGE("*** self__android_log_write, 444");


    // uint8_t* psrc = (uint8_t* )src__android_log_write;
    // uint8_t* porig = (uint8_t* )ori__android_log_write;
    
    // uint8_t sz_src[] = {0xEB, 0xFE};
    // nd_utils_patch_code(psrc + 0x27BF - 0x2790, sz_src, sizeof(sz_src));

    // nd_utils_patch_code(psrc, psrc, 1);
    // LOGD("pid: %d", getpid());


    ret = ori__android_log_write(prio, tag, msg);


    LOGE("*** self__android_log_write, end");

    return ret;
}

int do_dx_hook()
{
    // dx_utils_hook_debug(true);
    int ret = -1;
    void *libc_handle;
    
    // libc_handle = dlopen("libc.so", RTLD_LOCAL);
    if (libc_handle != NULL) {
        open("/data/local/tmp/abc.txt", 0, 0);
        LOGE("[do_dx_hook] start");
        // nd_debug(true);
        // hook_sys_func(libc_handle, "__open", (void *)self_open_func, (void **)&ori_open_func);
        // hook_sys_func(libc_handle, "__openat", (void *)self_openat_func, (void **)&ori_openat_func);
        void* src_mmap2 = (void* )dlsym(libc_handle, "__mmap2");
        LOGE("[__mmap2] src: %p", src_mmap2);
        TraceBuffer("__mmap2-src: ", (void* )src_mmap2, 20);

        dx_hook_hookfun((void* )src_mmap2, (void *)self_mmap2_func, (void **)&ori_mmap2_func);
        LOGE("[__mmap2] hook(%p, %p, &%p)", src_mmap2, self_mmap2_func, ori_mmap2_func);

        TraceBuffer("__mmap2-src: ", (void* )src_mmap2, 20);
        TraceBuffer("__mmap2-rep: ", (void* )self_mmap2_func, 40);
        TraceBuffer("__mmap2-ori: ", (void* )ori_mmap2_func, 40);

        // hook_sys_func(libc_handle, "munmap", (void *)self_munmap_func, (void **)&ori_munmap_func);
        // hook_sys_func(libc_handle, (char* )"write", (void *)self_write_func, (void **)&ori_write_func);
        LOGE("*** before hook write *** ");
        dx_hook_hookfun((void* )write, (void* )self_write_func, (void** )&ori_write_func);
        
        LOGE("*** after hook write *** ");
        return 0;
        // hook_sys_func(libc_handle, "read", (void *)self_read_func, (void **)&ori_read_func);
        // LOGE("");
        // LOGE("*** before hook*** ");
        // TraceBuffer("src: ", (void* )close, 20);
        // LOGE("");
    
        LOGE("*** before hook close *** ");
        hook_sys_func(libc_handle, (char* )"close", (void *)self_close_func, (void **)&ori_close_func);  
        LOGE("*** after hook close *** ");  
        // LOGE("hook_sys_func(%p, %p, &%p)", close, self_close_func, ori_close_func);    
        // hook_sys_func(libc_handle, "msync", (void *)self_msync_func, (void **)&ori_msync_func);
        // hook_sys_func(libc_handle, "pread64", (void *)self_pread64_func, (void **)&ori_pread64_func);
        // hook_sys_func(libc_handle, "pwrite64", (void *)self_pwrite64_func, (void **)&ori_pwrite64_func);
        
        // hook_sys_func(libc_handle, "ftruncate64", (void *)self_ftruncate64_func, (void **)&ori_ftruncate64_func);
        // hook_sys_func(libc_handle, "ptrace", (void *)self_ptrace_func, (void **)&ori_ptrace_func);

        // LOGE("");
        // LOGE("*** before hook*** ");
        // TraceBuffer("src: ", (void* )close, 20);
        // TraceBuffer("rep: ", (void* )self_close_func, 40);
        // TraceBuffer("ori: ", (void* )ori_close_func, 40);
        // LOGE("");


        // int fildes[2];
        // pipe(fildes);

        // LOGE("success, %d", close(fildes[0]));

        ret = 0;
    }


    void *liblog = dlopen("liblog.so", RTLD_LOCAL);
    if (liblog != NULL) {
        // nd_debug(true);

        // static int count = 0;
        // while (count++ < 20) {
        //     sleep(1);
        // }
        ANDROID_LOGE("111");
        LOGE("*** hook liblog.so start ***");
        src__android_log_write = (int (*)(int prio, const char *tag, const char *msg))dlsym(liblog, "__android_log_write");
        hook_sys_func(liblog, (char* )"__android_log_write", (void *)self__android_log_write, (void **)&ori__android_log_write);

        ANDROID_LOGE("222");
        ANDROID_LOGE("222");
        LOGE("*** hook liblog.so finish ***");

        ret = 0;
    }

    
    return ret;
}


int main(int argc, char **argv)
{
#if defined(__arm__)
    LOGE("test start, __arm__");
#elif defined(__i386__)
    LOGE("test start, __i386__");
#elif defined(__x86_64__)
    LOGE("test start, __x86_64__");
#elif defined(__arm64__) || defined(__aarch64__)
    LOGE("test start, __arm64__");
#endif

    LOGD("pid: %d", getpid());

#if 1
    void* handle = dlopen("libc.so", RTLD_NOW);
    uintptr_t sym1 = (uintptr_t )dlsym(handle, "__open");
    uintptr_t sym2 = (uintptr_t )dlsym(handle, "__get_tls");
    uintptr_t sym3 = (uintptr_t )dlsym(handle, "memswap");

    LOGD("__open: %p, hookable: %s", (void* )sym1, dx_hook_hookfun((void* )sym1, (void* )sym1, NULL) == DX_SIZE_NOT_ENOUGH ? "size not enough" : "success");
    LOGD("__get_tls: %p, hookable: %s", (void* )sym2, dx_hook_hookfun((void* )sym2, (void* )sym1, NULL) == DX_SIZE_NOT_ENOUGH ? "size not enough" : "success");
    LOGD("memswap: %p, hookable: %s", (void* )sym3, dx_hook_hookfun((void* )sym3, (void* )sym1, NULL) == DX_SIZE_NOT_ENOUGH ? "size not enough" : "success");
    return 0;

    l_image = (uintptr_t )get_module_base("test_hook");
    // LOGE("l_image: 0x%zX", l_image);

    LOGE("*** not hook ***");

    do_dx_hook();
    return 0;
#endif

    // function_all();

    // dx_hook_hookfun((void* )xxtea_encrypt_in, (void* )replace_xxtea_encrypt_in, (void** )&orig_xxtea_encrypt_in);

    LOGE("*** hooked ***");
    // function_all();

    LOGE("test end");
}
