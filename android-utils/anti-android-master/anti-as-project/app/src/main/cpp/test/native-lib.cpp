//
// Created by boyliang on 2017/5/26.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include "log.h"
#include "dx_anti.h"
#include "dx_anti_prefix.h"

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

    registerNativeMethods(env, "mobile/blue/com/anti_as_project/MainActivity", methods, NELEM(methods));
}


char *emu_info;

void anti_callback_impl(int64_t flag, void *args){
    err_log("flag : %d", flag);
    switch (flag) {
        case DXANTI_FLAG_DEBUG:
            break;
        case DXANTI_FLAG_EMULATOR:
            err_log("emulatorStr: %s", (char *)args);
            emu_info = (char *)args;
            break;
        case DXANTI_FLAG_FLAW_JANUS:
            break;
        case DXANTI_FLAG_INJECT:
            break;
        case DXANTI_FLAG_LOG:
            break;
        case DXANTI_FLAG_MEMDUMP:
            break;
        case DXANTI_FLAG_MULTIRUN:
            break;
        case DXANTI_FLAG_ROOT:
            break;
        case DXANTI_FLAG_VIRTUAL_APP:
            break;
        default:
            break;
    }
}



jstring MainActivity_stringFromJNI(JNIEnv *env, jobject obj, jobject jContext) {

    std::string hello = "Hello from C++";

    err_log(" ----- anti start ----- ");

    DXAnti_monitor_init(jvm, jContext);
    DXAnti_monitor_start(DXANTI_FLAG_DEBUG | DXANTI_FLAG_VIRTUAL_APP);

    bool check = DXAnti_monitor_check();
    err_log(" ----- anti DXAnti_monitor_check %d ----- ", check);

#ifdef ANTI_RETURN_ON
    DXAnti_set_monitor_callback(anti_callback_impl);
#endif

    err_log(" ----- anti end ----- ");

    if (emu_info != NULL) {
        return env->NewStringUTF(emu_info);
    }

    return env->NewStringUTF("");
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    err_log(" ----- JNI_OnLoad ----- ");
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    jvm = vm;

    registerDXRiskJNI(env);

    return JNI_VERSION_1_4;
}
