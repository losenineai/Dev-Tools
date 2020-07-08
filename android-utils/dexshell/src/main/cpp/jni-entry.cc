#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>

#include "common.h"
#include "HookProcs.h"
#include "decrypt.h"

static JNINativeMethod getMethods[] =
        {
            {"initNative", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", (void*)initNative},
            {"initDexOptDexLoad", "(II)I", (void*)initDexOptDexLoad},
            {"initNormalDexLoad", "(II)I", (void*)initNomalDexLoad},
            {"getSrcAddr", "()[B", (void*)getAddr},
            {"cleanup", "()V", (void*)cleanup}
        };

static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        return 0;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
    {
        return 0;
    }
    return 1;
}

static int registerNatives(JNIEnv* env)
{
    static const char *jniClassName = "com/security/shell/ShellSupporter";
    if (!registerNativeMethods(env, jniClassName, getMethods, sizeof(getMethods) / sizeof(getMethods[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGI("in JNI_OnLoad, =======start");
    //测试android5.0上不打开这个参数多线程makeDex会失败，原因不明
    //多线程makeDex已经兼容，在后台线程make失败会自动转换为单线程make
    prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);

    jint result = -1;

    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    if(!registerNatives(env))
        return JNI_ERR;

    return JNI_VERSION_1_4;
}