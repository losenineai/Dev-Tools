#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include "log.h"
#include "jni_utils.h"

void callJava() {
    JniUtils jni;
    jstring jstr = jni.String2jstring("123");
    LOGD("jstr:%p", jstr);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnLoad =======start");

    jint result = -1;

    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    JniUtils::Init(vm);

    callJava();

    LOGD("JNI_OnLoad =======end");
    return JNI_VERSION_1_4;
}