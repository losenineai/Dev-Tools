#ifndef COMMON_H_
#define COMMON_H_

#include <android/log.h>
#include <stdlib.h>

#define TAG "dex-shell"

#if defined (__i386__) || defined (__arm__)
#define DX_RUN_ON_32
#endif

#ifdef STACK
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unwind.h>

using namespace std;

#define PRINT_STACK(x) \
    const size_t max = 30; \
    void *buffer[max]; \
    std::ostringstream oss; \
    dumpBacktrace(oss, buffer, captureBacktrace(buffer, max));  \
    LOGI("%s:%s", x, oss.str().c_str());
#else
#define PRINT_STACK(x)
#endif

struct __attribute__((__aligned__(4), __packed__)) DexConfig {
    uint32_t id;
    uint32_t len;
    uintptr_t addr;
};

struct DexPointer {
    DexConfig* config;
    DexPointer* next;
    char* fileName;
};

inline void LOGD(const char *fmt, ...) {
#ifdef DEBUG
    va_list vl;
    va_start(vl, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, vl);
    va_end(vl);
#endif
}

inline void 8LOGI(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    __android_log_vprint(ANDROID_LOG_INFO, TAG, fmt, vl);
    va_end(vl);
}

inline void LOGE(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    __android_log_vprint(ANDROID_LOG_ERROR, TAG, fmt, vl);
    va_end(vl);
}

const char *getHex(char *buf, size_t outputLen, void *src, size_t len);

int get_fname_by_fd(int fd, char *outname, size_t sz);
#ifdef STACK
size_t captureBacktrace(void **buffer, size_t max);
void dumpBacktrace(std::ostream &os, void **buffer, size_t count);
#endif

#endif