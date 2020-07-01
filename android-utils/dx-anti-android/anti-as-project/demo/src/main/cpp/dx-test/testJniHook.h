//
// Created by weaiken on 2019-11-12.
//



#include <jni.h>
#include <stdlib.h>
#include <android/log.h>
#include <dlfcn.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/system_properties.h>
#include <stdio.h>

#define TAG "HOOKCAMERA"
#define JAVA_CLASS "com/dingxiang/mobile/demo/MainActivity"


#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,  TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved);

//jstring MainActivity_testHook(JNIEnv *env, jobject obj, jobject jContext);
