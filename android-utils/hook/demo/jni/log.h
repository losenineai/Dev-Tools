#include <android/log.h>
#include <string>


#if 0
#define LOG_TAG "dx-native"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#else
#define LOG_TAG "dx-native"
#define ANDROID_LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) printf("[" LOG_TAG "] " fmt "\n", ##args)
#define LOGE(fmt, args...) printf("[" LOG_TAG "] " fmt "\n", ##args)
#endif

static void TraceBuffer(const char* tag, const void* buffer, size_t size)
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