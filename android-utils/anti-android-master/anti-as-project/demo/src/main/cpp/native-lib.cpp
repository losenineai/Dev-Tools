//
// Created by boyliang on 2017/5/26.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>
#include <dx_anti.h>

//include "platform.h"
#include "log.h"

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define JSTRING "Ljava/lang/String;"
#define JCONTEXT "Landroid/content/Context;"

JavaVM *jvm;

/** begin of register methods  **/
static void
registerNativeMethods(JNIEnv *env, const char *claxxname, JNINativeMethod *methods, int nmethods) {
    jclass claxx = env->FindClass(claxxname);

    if (claxx == NULL) {
        err_log("Can't find class %s.", claxxname);
        exit(-1);
    }

    env->RegisterNatives(claxx, methods, nmethods);

    env->DeleteLocalRef(claxx);
}

jstring
MainActivity_stringFromJNI(JNIEnv *env, jobject obj, jobject jContext);

inline void registerDXRiskJNI(JNIEnv *env) {

    JNINativeMethod methods[] = {
            {
                    "stringFromJNI",
                    "(" JCONTEXT ")" JSTRING,
                    (void *) MainActivity_stringFromJNI
            }
    };

    registerNativeMethods(env, "com/dingxiang/mobile/demo/MainActivity", methods, NELEM(methods));
}

jstring MainActivity_stringFromJNI(JNIEnv *env, jobject obj, jobject jContext) {

    std::string hello = "Hello from C++";

    DXAntiRetStatus ret = DXAnti_monitor_init(jvm, NULL);
    err_log(" ----- anti DXAnti_monitor_init ret %d----- ", ret);

    if (ret == INIT_SUCCESS) {
        DXAnti_monitor_start(DXANTI_FLAG_DEBUG | DXANTI_FLAG_EMULATOR | DXANTI_FLAG_FLAW_JANUS | DXANTI_FLAG_INJECT | DXANTI_FLAG_MEMDUMP | DXANTI_FLAG_MULTIRUN | DXANTI_FLAG_VIRTUAL_APP);
    }

    bool check = DXAnti_monitor_check();
    err_log(" ----- anti DXAnti_monitor_check check %d----- ", check);

    return env->NewStringUTF(hello.c_str());
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    jvm = vm;

    registerDXRiskJNI(env);

    return JNI_VERSION_1_4;
}
