//
// Created by boyliang on 2017/5/26.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>

#include "jni_utils.h"
#include "stee.h"
#include "DXRiskInter.h"
#include "DXEnvCheck.h"
#include "DXRiskInterManager.h"
#include "DXLog.h"
#include "storage_utils.h"
#include "dex-vmp-bridge.h"

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define JSTRING "Ljava/lang/String;"
#define JCONTEXT "Landroid/content/Context;"
#define JBARRAY "[B"
#define JMAP "Ljava/util/HashMap;"


/** begin of register methods  **/
static void
registerNativeMethods(JNIEnv *env, const char *claxxname, JNINativeMethod *methods, int nmethods) {
    jclass claxx = env->FindClass(claxxname);

    if (claxx == NULL) {
        LOGE("Can't find class %s.", claxxname);
        exit(-1);
    }

    env->RegisterNatives(claxx, methods, nmethods);

    delete_ref(env, claxx);
}

/* DXRisk */
jlong
DXRisk_createObjectNative(JNIEnv *env, jclass clz);
void
DXRisk_setupNative(JNIEnv *env, jobject obj, jlong id, jobject jContext);
jstring
DXRisk_getTokenNative(JNIEnv *env, jobject obj, jlong id, jstring appId, jobject extendparams, jboolean bGetFullToken);

inline void registerDXRiskJNI(JNIEnv *env) {

    JNINativeMethod methods[3];
    methods[0] = {
            "createObjectNative",
            "()J",
            (void *) DXRisk_createObjectNative
    };

    methods[1] = {
            "getTokenNative",
            "(J" JSTRING JMAP "Z)" JSTRING,
            (void *) DXRisk_getTokenNative
    };

    methods[2] = {
            "setupNative",
            "(J" JCONTEXT ")V",
            (void *) DXRisk_setupNative
    };

//    registerNativeMethods(env, "com/dingxiang/mobile/risk/DXRiskInternal", methods, NELEM(methods));
    registerNativeMethods(env, CUSTOMIZE_DXRISK_INTERNAL, methods, NELEM(methods));
}

jlong
DXRisk_createObjectNative(JNIEnv *env, jclass clz) {

    return reinterpret_cast<jlong>(DXRiskInterManager::GetInstance());
}

void DXRisk_setupNative(JNIEnv *env, jobject obj, jlong id, jobject jContext) {

    jobject glbalContext = convert_to_global_ref(env, jContext);
    AndroidRuntime::initContext(glbalContext);

    DXEnvCheck::GetInstance()->Setup();
}

jstring DXRisk_getTokenNative(JNIEnv *env, jobject obj, jlong id, jstring appId, jobject extendparams, jboolean bGetFullToken) {

    DXRiskInterManager *manager = reinterpret_cast<DXRiskInterManager *>(id);

    string cpp_appId = jstring_to_string(env, appId);
    DXRiskInter *risk = manager->Get(cpp_appId);

    map<string, string> map_extendparams = jmap_to_map(env, extendparams);
    risk->Init(cpp_appId, map_extendparams);

    string cpp_token = risk->GetToken(bGetFullToken);
    jstring j_token = string_to_jstring(env, cpp_token);

    return j_token;
}

/* b */
jboolean
StorageUtils_putNative(JNIEnv *env, jclass clz, jstring key, jstring value, jint scope);

jstring
StorageUtils_getNative(JNIEnv *env, jclass clz, jstring key, jint scope, jstring defValue);

jboolean
StorageUtils_removeNative(JNIEnv *env, jclass clz, jstring key, jint scope);

inline void registerStorageJNI(JNIEnv *env) {

    JNINativeMethod methods[3];
    methods[0] = {
            "pn",
            "(" JSTRING JSTRING "I)Z",
            (void *) StorageUtils_putNative
    };

    methods[1] = {
            "gn",
            "(" JSTRING "I" JSTRING ")" JSTRING,
            (void *) StorageUtils_getNative
    };

    methods[2] = {
            "rn",
            "(" JSTRING "I)Z",
            (void *) StorageUtils_removeNative
    };


    // b为StorageUtils的混淆
    // registerNativeMethods(env, "com/dingxiang/mobile/risk/dx/b", methods, NELEM(methods));
    registerNativeMethods(env, CUSTOMIZE_STORAGE_UTILS, methods, NELEM(methods));
}

jboolean
StorageUtils_putNative(JNIEnv *env, jclass clz, jstring key, jstring value, jint scope) {

    LOGR("%d", TAG_JNI_BINDER + 103);

    string cpp_key = jstring_to_string(env, key);
    string cpp_value = jstring_to_string(env, value);

    return StorageUtils::Put(cpp_key, cpp_value, scope);
}

jstring
StorageUtils_getNative(JNIEnv *env, jclass clz, jstring key, jint scope, jstring defValue) {

    LOGR("%d", TAG_JNI_BINDER + 101);

    string cpp_key = jstring_to_string(env, key);
    string cpp_defValue = jstring_to_string(env, defValue);

    string value = StorageUtils::Get(cpp_key, scope, cpp_defValue);

    return string_to_jstring(env, value);
}

jboolean
StorageUtils_removeNative(JNIEnv *env, jclass clz, jstring key, jint scope) {

    LOGR("%d", TAG_JNI_BINDER + 102);

    string cpp_key = jstring_to_string(env, key);

    return StorageUtils::Remove(cpp_key, scope);
}

STEE
extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    // vmp加固必须调用，debug开发需屏蔽才可以运行
#ifndef DEBUG
    dex_vmp_start_engine(env);
#endif

    AndroidRuntime::init(vm);
    jni_init();

    registerDXRiskJNI(env);

    registerStorageJNI(env);

    return JNI_VERSION_1_4;
}
