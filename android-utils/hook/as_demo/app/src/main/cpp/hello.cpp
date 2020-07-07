#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include "dx_hook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "TestFunc1.h"
#include "TestFunc2.h"
#include "TestFunc3.h"
#include "base64.h"
#include "xxtea.h"
#include "dx_hook.h"

#if 0
#define LOG_TAG "dx-native"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
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

    LOGE("*** not hook ***");
    function_all();

    hook_function((void* )xxtea_encrypt_in, (void* )replace_xxtea_encrypt_in, (void** )&orig_xxtea_encrypt_in);

    LOGE("*** hooked ***");
    function_all();

    LOGE("test end");
}
