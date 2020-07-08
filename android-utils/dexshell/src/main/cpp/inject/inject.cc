//
// Created by my on 18-11-20.
//

#include <android/log.h>
#include <stdlib.h>
#include "HookUtils.h"
#include "ElfUtils.h"
#include "common.h"
#include "decrypt.h"
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/system_properties.h>
#include <string.h>

static int (*ori_close_func)(int fildes) = close;
static int (*ori_android_fdsan_close_with_tag)(int fildes, uint64_t) = 0;
static int (*ori_open_func)(const char *path, int flags, ...) = open;
static ssize_t (*ori_write_func)(int fd, const void *buf, size_t len) = write;
static void (*ori_exit)(int status) = exit;


static int gSdkVersion = 0;
static char sBaseDex[256] = {0};
static const char *sBaseOat = "app_csn0/.opt/";
static char sApkPath[256] = {0};
static char sTmpDir[256] = {0};
static char sBaseVdex[256] = {0};

static int sEncFd = 0;
static size_t sMaxEnc = 1024;

static bool decrypt_dex_file(const char *path, const char *outPath) {
    int fd = ori_open_func(path, O_RDONLY);

    char buf[1024] = {0};
    read(fd, buf, 3);
    if (buf[0] == 'd' && buf[1] == 'e' && buf[2] == 'x') {
        //明文不需要解密
        ori_close_func(fd);
        return false;
    } else {
        LOGI("in dec...");
        lseek(fd, 0, SEEK_SET);
        int fdOut = ori_open_func(outPath, O_CREAT | O_WRONLY, 0666);
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        lock.l_pid = getpid();

        int ret = fcntl(fdOut, F_SETLKW, &lock);
        LOGI("flock return %d", ret);
        if (ret < 0) {
            LOGI("flock error %s", strerror(errno));
        }
        ssize_t count;
        while (true) {
            count = read(fd, buf, sizeof(buf));
            if (count <= 0) {
                break;
            }
            simple_xor_decrypt((unsigned char*)buf, (size_t)count);
            write(fdOut, buf, (size_t)count);
        }

        int r = fcntl(fdOut, F_UNLCK, &lock);
        LOGI("flock unlock return %d", r);
        LOGI("before close %d", fd);
        ori_close_func(fd);
        LOGI("flock close fd:%d...", r);
        LOGI("flock unlock return %d 1", r);
        LOGI("before close 2 %d", fdOut);
        ori_close_func(fdOut);
        LOGI("flock unlock return %d 2", r);
        return true;
    }

}

static void _encOat(int fd, int maxEnc) {

    fsync(fd);
    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));
    struct stat st = {0};
    fstat(fd, &st);
    LOGI("before map fd=%d, sz=%u", fd, st.st_size);
    //lseek(fd, 0, SEEK_END);
    // 验证 poc， 解密
    if (st.st_size > 0) {
        char *addr = (char *) mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (addr != MAP_FAILED) {
            LOGI("after map %p", addr);
            void *dynsym = 0;
            void *dynstr = 0;
            size_t nsyms = 0;
            getDynFromSo(addr, dynsym, dynstr, nsyms);
            LOGI("before find1 dynsym=%p, dynstr=%p, nsyms=%d", dynsym, dynstr, nsyms);
            void *begin = findDynSymAddr(addr, "oatdata", dynsym, dynstr, nsyms);
            LOGI("after find1");
            void *end = findDynSymAddr(addr, "oatexec", dynsym, dynstr, nsyms);
            LOGI("after find2");
            if (!begin || !end) {
                LOGI("warning enc oatfile has invalid begin=%p or end=%p skip encrypt", begin, end);
                return;
            }

            size_t dataRange = (size_t) end - (size_t) begin;
            LOGI("oatdata:%p, oatexec=%p, range=%u", begin, end, dataRange);

            size_t sz = maxEnc < dataRange ? maxEnc : dataRange;

            char *base = (char *) begin;

            char hex1[100] = "", hex2[100] = "";
            getHex(hex1, sizeof(hex1), base, sz);
            getHex(hex2, sizeof(hex2), base + sz - 1 - 100, 100);

            simple_xor_decrypt((unsigned char*)base, sz);

            getHex(hex1, sizeof(hex1), base, sz);
            getHex(hex2, sizeof(hex2), base + sz - 1 - 100, 100);
            LOGI("inject oat after enc filename=%s,sz=%d,hex_head=%s,hex_end=%s", name, sz,
                 hex1, hex2);
            msync(addr, sz, MS_SYNC);
            munmap(addr, sz);
        }
        else {
            LOGI("mmap error return %s", strerror(errno));
        }

    }
}

static void _encVdex(int fd, int maxEnc) {
    fsync(fd);
    struct stat st = {0};
    fstat(fd, &st);
    //随便跳过一个页，跳过vdex头，以防影响校验
    int fixSkip = getpagesize();
    //太小也没必要加密了
    if (fixSkip < st.st_size) {
        LOGI("before map fd=%d, sz=%u", fd, st.st_size);
        char *addr = (char *) mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                   0);
        if (addr == MAP_FAILED) {
            LOGI("mmap error return %s", strerror(errno));
        }

        void *begin = addr + fixSkip;
        void *end = addr + st.st_size;
        size_t dataRange = (size_t )end - (size_t )begin;
        size_t sz = maxEnc < dataRange ? maxEnc : dataRange;

        LOGI("vdex enc base:%p, sz=%d", begin, sz);
        char *base = (char*)begin;
        simple_xor_decrypt((unsigned char*)base, sz);
        msync(addr, sz, MS_SYNC);
        munmap(addr, sz);
    }
}

static void close_common(int fd) {
    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    if (gSdkVersion < 26) {
        //8.0以下使用oat路径
        if (strstr(name, sBaseOat)) {
            if (fd == sEncFd) {
                sEncFd = 0;
            }
            LOGI("inject [%s] target path %d %s", __func__, fd, name);
            PRINT_STACK(__func__);
            _encOat(fd, sMaxEnc);
        }
    }
    else {
        //8.0或者以上使用vdex加密
        if (strstr(name, sBaseVdex) && strstr(name, ".vdex")) {
            LOGI("vdex enc %s...", name);
            _encVdex(fd, sMaxEnc);
        }
    }

}

static int self_android_fdsan_close_with_tag(int fd, uint64_t tag) {

    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    close_common(fd);
    int r = ori_android_fdsan_close_with_tag(fd, tag);
    return r;
}
/**
 * close
 */
static int self_close_func(int fildes) {

    close_common(fildes);
    int r = ori_close_func(fildes);
    return r;
}

static int self_open_func(const char *path, int flags, ...) {

    //LOGI("inject in hook open %s, flag=0x%08x", path, flags);
    //PRINT_STACK(__func__);
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = (mode_t) va_arg(args, int);
        va_end(args);
    }

    if (strstr(path, sBaseDex)) {
        // 验证 poc， 解密
        //PRINT_STACK(__func__);
        LOGI("inject self_open_func find target dex %s", path);
        char fakePath[260] = {0};
        const char *p = strrchr(path, '/');
        sprintf(fakePath, "%s/.%s.tmp", sTmpDir, p + 1);

        LOGI("inject self_open_func fake:%s", fakePath);
        if (decrypt_dex_file(path, fakePath)) {
            LOGI("inject self_open_func before open fake:%s", fakePath);
            int fd = ori_open_func(fakePath, flags, mode);
            LOGI("inject self_open_func dec dex path:%s fakePath:%s fd:%d", path, fakePath, fd);
            unlink(fakePath);
            LOGI("inject self_open_func after open fake:%s", fakePath);
            return fd;
        }
    }
    int fd = ori_open_func(path, flags, mode);

    return fd;
}

static ssize_t self_write_func(int fd, const void *buf, size_t len) {

    char name[256] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    ssize_t ret;

    if (strstr(name, "app_csn0/.opt/")) {
        sEncFd = fd;
    }
    ret = ori_write_func(fd, buf, len);
    return ret;
}

static void self_exit(int status){

    LOGI("inject self_exit called!");

    if (status == 0 && sEncFd != 0){
        int fd = sEncFd;
        _encOat(fd, sMaxEnc);
    }

    ori_exit(status);
    return;
}

static bool try_hook_android10() {

    void *libArtBase = getTargetSI("libartbase.so");
    if (!libArtBase) {
        return false;
    }
    replaceFunc(libArtBase, "open", (void *) self_open_func, (void **) &ori_open_func);
    replaceFunc(libArtBase, "android_fdsan_close_with_tag", (void*)self_android_fdsan_close_with_tag, (void**)&ori_android_fdsan_close_with_tag);
    return true;
}

//#include "dx_hook.h"

static void _init() {
    const char *pkgName = getenv("pkgName");
    const char *apkPath = getenv("apkPath");
    const char *maxEncSize = getenv("encSize");

    if (NULL != maxEncSize || *maxEncSize != '\0'){
        sMaxEnc = (size_t )atoi(maxEncSize);
    }

    sprintf(sBaseDex, "app_csn0/.unzip/classes");
    strcpy(sApkPath, apkPath);
    sprintf(sTmpDir, "/data/data/%s/.tmp_d", pkgName);
    mkdir(sTmpDir, 0755);

    sprintf(sBaseVdex, "app_csn0/.unzip/oat/");

    LOGI("test inject load enc_return pkgName:%s, apkPath:%s!!!!!!!", pkgName, apkPath);

    /*
    dx_hook_hookfun((void*)open, (void*)self_open_func, (void**)&ori_open_func);
    void *p = dlopen("libc.so", RTLD_NOW);
    void *f = dlsym(p, "android_fdsan_close_with_tag");
    dx_hook_hookfun((void*)close, (void*)self_close_func, (void**)&ori_close_func);
    dx_hook_hookfun(f, (void*)self_android_fdsan_close_with_tag, (void**)&ori_android_fdsan_close_with_tag);
     */


    char sdkchar[16];
    __system_property_get("ro.build.version.sdk", sdkchar);
    int sdkVersion = atoi(sdkchar);
    gSdkVersion = sdkVersion;
    if (sdkVersion >= 28) {
        if (try_hook_android10()) {
            return;
        }
    }

    void *libTarget = 0;
    if (sdkVersion > 19) {
        libTarget = getTargetSI("libart.so");
        if (!libTarget) {
            libTarget = getTargetSI("libaoc.so");
        }
    }

    replaceFunc(libTarget, "open", (void *) self_open_func, (void **) &ori_open_func);
    if (sdkVersion > 22) {
        replaceFunc(libTarget, "close", (void *) self_close_func, (void **) &ori_close_func);
    } else if (sdkVersion > 19){
        void* dex2oat = getTargetSI("dex2oat");
        replaceFunc(dex2oat, "exit", (void*)self_exit, (void**)&ori_exit);
        replaceFunc(libTarget, "write", (void*)self_write_func, (void**)&ori_write_func);
        replaceFunc(libTarget, "close", (void *) self_close_func, (void **) &ori_close_func);
    }
}

__attribute__((constructor)) static void test() {
    _init();
}
