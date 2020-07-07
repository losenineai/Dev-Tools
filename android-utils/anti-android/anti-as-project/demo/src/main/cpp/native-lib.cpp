//
// Created by boyliang on 2017/5/26.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>
#include <dx_anti.h>
#include <log.h>
#include <android/log.h>
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


jstring
MainActivity_testHook(JNIEnv *env, jobject obj, jobject jContext);

jstring MainActivity_testHook(JNIEnv *env, jobject obj, jobject jContext) {

    std::string hello = "Hello from Weaiken";

//    DXAnti_ResultCallback callback = mycallback;
//    DXAnti_set_monitor_callback(callback);

    JNIEnv* newEnv = DXAnti_detect_jni_hook_setup(env , (uintptr_t )__builtin_return_address(0));
    return newEnv->NewStringUTF(hello.c_str());
//  return env->NewStringUTF(hello.c_str());
}



inline void registerDXRiskJNI(JNIEnv *env) {

    JNINativeMethod methods[] = {
            {
                    "stringFromJNI",
                    "(" JCONTEXT ")" JSTRING,
                    (void *) MainActivity_stringFromJNI
            }
            , {
                    "testHook",
                    "(" JCONTEXT ")" JSTRING,
                    (void *) MainActivity_testHook
            }
    };

    registerNativeMethods(env, "com/dingxiang/mobile/demo/MainActivity", methods, NELEM(methods));
}

void mycallback(int64_t flag, void *args){
    err_log("==========flag:%lld", flag);

    if(flag == DXANTI_FLAG_ROOT){
        err_log("detect-root");
    }else if(flag == DXANTI_FLAG_DEBUG){
        err_log("detect-debug");
    }else if(flag == DXANTI_FLAG_EMULATOR){
        err_log("detect-emulator");
    }else if(flag == DXANTI_FLAG_FLAW_JANUS){
        err_log("detect-flaw-janus");
    }else if(flag == DXANTI_FLAG_INJECT){
        err_log("detect-inject");
    }else if(flag == DXANTI_FLAG_MEMDUMP){
        err_log("detect-memdump");
    }else if(flag == DXANTI_FLAG_MULTIRUN){
        err_log("detect-multirun");
    }else if(flag == DXANTI_FLAG_LOG){

    }

    if(args != NULL){
        err_log("==========args:%s", (char*)args);
    }

}
typedef void (*DXAnti_ResultCallback)(int64_t flag, void *args);

jstring MainActivity_stringFromJNI(JNIEnv *env, jobject obj, jobject jContext) {

    std::string hello = "Hello from C++";

//    DXAnti_ResultCallback callback = mycallback;
//    DXAnti_set_monitor_callback(callback);
    JNIEnv* newEnv = DXAnti_detect_jni_hook_setup(env , (uintptr_t )__builtin_return_address(0));
    DXAnti_monitor_init(jvm, jContext );

    err_log(" ----- anti ----- ");

    DXAnti_monitor_start(
        DXANTI_FLAG_LOG |
    DXANTI_FLAG_ROOT |
    DXANTI_FLAG_DEBUG |
    DXANTI_FLAG_EMULATOR |
    DXANTI_FLAG_FLAW_JANUS |
    DXANTI_FLAG_INJECT |
    DXANTI_FLAG_MEMDUMP |
    DXANTI_FLAG_MULTIRUN |
    DXANTI_FLAG_THREAD |
    DXANTI_FLAG_HOOK |
    DXANTI_FLAG_JNIHOOK
    );

    bool check = DXAnti_monitor_check();

    err_log(" ----- anti DXAnti_monitor_check check %d----- ", check);


    return newEnv->NewStringUTF(hello.c_str());
//  return env->NewStringUTF(hello.c_str());
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    jvm = vm;

    registerDXRiskJNI(env);

//    DXMethod methods[] = {
//            {"com/dingxiang/mobile/demo/MainActivity" , "stringFromJNI" , "()Ljava/lang/String;" }
//    };

    DXAnti_detect_jni_hook_init(vm, env);
//    DXAnti_detect_jni_hook_add_methods(methods , sizeof(methods) / sizeof(methods[0]));


    return JNI_VERSION_1_4;
}
