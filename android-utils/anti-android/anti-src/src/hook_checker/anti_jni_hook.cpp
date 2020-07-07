//
//  DXJavaHookChecker.cpp
//  JavaHookChecker
//
//  Created by mt on 2017/10/18.
//  Copyright © 2017年 mt. All rights reserved.
//

#include "include/anti_jni_hook.h"
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "jni_utils.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"
#include "include/env-chain.h"
#include "include/jni-proxy.h"
#include "include/DXMethodIndexTable.h"
#include "include/DXCommon.h"
#include "include/DXEnv.h"
#include "include/DXArrayList.h"
#include "anti_result_handler.h"
#include "anti_crash_log_prefix.h"
#include "utils_anti_log.h"
#include "anti_utils.h"

// ************************** check return-value **************************
STEE
bool DXJavaHookChecker_IsReturnValueOk(uintptr_t retAddr) {
    if (DXIsAddrInSystem(retAddr)) {
        return true;
    }
    return false;
}

// ************************** check function-list **************************
STEE
bool DXJavaHookChecker_IsEnvFuncListOk(JNINativeInterface* jniNativeInterface)
{
    if (jniNativeInterface == NULL) {
        return false;
    }

    bool flag = true;
    uintptr_t* pFunc = (uintptr_t* )jniNativeInterface;

    int count = 233; // size / sizeof(void*);

//    for (int i = 0; i < count; ++i) {
//        LOGD("fl[%d]: %p", i, pFunc[i]);
//    }
    // 去除前4个和最后一个
    for (int i = 4; i < count - 1; ++i) {
        if (!DXIsDvmVadd(pFunc[i])) {
            anti_info_log("dxer, fl[%d]: %p", i, (void* )pFunc[i]);
            flag = false;
            break;
        }
    }

    return flag;
}

STEE
bool DXJavaHookChecker_CheckEnvFuncList(JNINativeInterface* jniNativeInterface)
{
    if (!DXJavaHookChecker_IsEnvFuncListOk(jniNativeInterface)) {
//        DXAbort();
        return false;
    }
    return true;
}

//bool DXJavaHookChecker_CheckEnvFuncList(JniEnvChain* jniEnvChain)
//{
//    return DXJavaHookChecker_CheckEnvFuncList(DXGetJNINativeInterface(jniEnvChain));
//}



//#define CHECK_FUNCTION_LIST(_jniEnvProxy_) \
//{ \
//    if (!DXJavaHookChecker_IsEnvFuncListkOk((JniEnvProxy* )(_jniEnvProxy_))) { \
//        LOGD("[%s(%d)] %p: dxer!", __FUNCTION__, __LINE__, l_callTable); \
//        DXJavaHookChecker_Abort(); \
//    } \
//}


// ************************** check method-nativalize **************************
static DXArrayList<DXMethod> l_dxMethods;

STEE
bool DXJavaHookChecker_CheckMethodNativalize(JNIEnv* env)
{
    bool ret = true;
    jclass Modifier;
    jobject modifier;
    bool isNative = false;

    // Modifier modifier = new Modifier();
    Modifier = DXFindClass(env, "java/lang/reflect/Modifier");
    modifier = DXNewObjectByName(env, Modifier, "<init>", "()V");
    DXDeleteGlobalRef(env, Modifier);

//    anti_info_log("modifier: %p", modifier);

    // while 循环检测
    for (int i = 0; i < l_dxMethods._currentNum; ++i)
    {
        DXMethod* method = &l_dxMethods._array[i];

        isNative = DXIsMethodNative(
            env, 
            modifier, 
            method->clsName, 
            method->name,
            method->sig);

        // LOGD("%s %s native method", method->name, isNative ? "is" : "is not");
        if (isNative) {
//            LOGD("\"%s\" is abnormal!", method->name);
//            DXAbort();
            ret = false;
            break;
        }
    }

    DXDeleteGlobalRef(env, modifier);
    return ret;
}

// ************************** check method-nativalize **************************
STEE
bool DXJavaHookChecker_CheckAll(JniEnvChain* jniEnvChain)
{
//    bool flag = true;
    JNIEnv* env = DXGetJNIEvn(jniEnvChain);
    JNINativeInterface* jniNativeInterface = *(JNINativeInterface** )env;

    // check call table
    anti_info_log("check call table");
    if (!DXJavaHookChecker_CheckEnvFuncList(jniNativeInterface)) {
//        flag = true;
         return false;
    }

    // check call content
    anti_info_log("check call content");
    if (!DXJavaHookChecker_CheckJniMethodContent(jniNativeInterface)) {
//        flag = true;
         return false;
    }

    // check JAVA method is Hook : native-methods
    anti_info_log("check native-methods");
    if (l_dxMethods._currentNum > 0){
        if (!DXJavaHookChecker_CheckMethodNativalize(env)) {
//        flag = true;
            return false;
        }
    }
    return true;
}





// ************************** check function-list & return-value while jni function is calling **************************




// ************************** exoport functions **************************
// #define DXJavaHookChecker_Init sub_3890
// #define DXJavaHookChecker_AddImportMethods sub_3894
// #define DXJavaHookChecker_Setup sub_3898
STEE
void DXAnti_detect_jni_hook_init(JavaVM* vm, JNIEnv* env)
{
//    JNINativeInterface* jniNativeInterface = *(JNINativeInterface** )env;
    // LOGD("cvmv: %p", jniNativeInterface->CallVoidMethodV);
    // LOGD("cvmv: %p", jniNativeInterface->CallVoidMethodA);
    // LOGD("cvmv: %p", jniNativeInterface->CallVoidMethod);

    // initalize dvm base
    anti_info_log("initalize dvm base");
    uintptr_t dvmBase = DXInitalizeDvmBase();
    anti_info_log("dvmBase = %p", (void* )dvmBase);
    if (dvmBase == 0) {
        //todo response Hook True
//        DXAbort();
        AntiResult result = {AntiType::AT_HOOK,
                             ANTI_DEBUG_ERR_HOOK_JNI + 11};
//        LOG_WARNING("%s", isEmuStr.c_str());
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    // initalize method-index-table
    anti_info_log("initalize dvm base");
    if (!DXMethodIndexTable_Initalize(env)) {
        //todo response Hook True
        AntiResult result = {AntiType::AT_HOOK,
                             ANTI_DEBUG_ERR_HOOK_JNI + 12};
//        LOG_WARNING("%s", isEmuStr.c_str());
        AntiResultHandler::getInstance()->HandleResult(&result);
//        DXAbort();
    }
    // initalize dvm proxy
    anti_info_log("initalize dvm proxy");
//    cvm.SetVM(vm);
}

STEE
void DXAnti_detect_jni_hook_add_methods(DXMethod methods[] , size_t size)
{
//    size_t length = (sizeof(methods) / sizeof(DXMethod[0]));
    for (int i = 0; i < size; ++i)
    {
        l_dxMethods.add(&methods[i]);
    }
}

STEE
JNIEnv* DXAnti_detect_jni_hook_setup(JNIEnv *env, uintptr_t retAddr)
{
    JNIEnv* newEnv = NULL;

    // check return-value

    anti_info_log("check return-value");
    if (!DXJavaHookChecker_IsReturnValueOk(retAddr)) {
        anti_info_log("%p: dxer", (void* )retAddr);
//        DXAbort();
//todo response Hook True
        AntiResult result = {AntiType::AT_HOOK,
                             ANTI_DEBUG_ERR_HOOK_JNI + 1};
//        LOG_WARNING("%s", isEmuStr.c_str());
        AntiResultHandler::getInstance()->HandleResult(&result);
    }
    // proxy env
    newEnv = DXAnti_AndroidRuntime::cvm.proxy(env);
//    newEnv = cvm.proxy(env);
    //屏蔽每次调用都要检测全部方法是否Hooked
//    // check call-table & method-content & check native-methods
//    bool reb =  DXJavaHookChecker_CheckAll((JniEnvChain* )newEnv);
//    if (!reb){
//        AntiResult result = {AntiType::AT_HOOK,
//                             ANTI_DEBUG_ERR_HOOK_JNI + 2};
//        AntiResultHandler::getInstance()->HandleResult(&result);
//    }

    return newEnv;
}

STEE
void *DXAnti_detect_jni_hook(void *arg) {
    DXAnti_JNIEnvGuard guard;
    anti_err_log("DXAnti_detect_jni_hook start");
    JNIEnv *newEnv = NULL;
    JavaVM *g_JavaVm = DXAnti_AndroidRuntime::cvm.GetProxyedVM();
    jint re = g_JavaVm->GetEnv((void **) &newEnv, JNI_VERSION_1_4);
    anti_err_log("DXAnti_detect_jni_hook -- %d" , re);
    if (re != JNI_OK) {
        re = DXAnti_AndroidRuntime::cvm.GetProxyedVM()->AttachCurrentThread((JNIEnv**) &g_JavaVm , &newEnv );
        if (re < 0){
//            anti_err_log("DXAnti_detect_jni_hook null env");
            return NULL;
        }
    }

    while (1){
        anti_info_log("DXAnti_detect_jni_hook check");
        bool reb =  DXJavaHookChecker_CheckAll((JniEnvChain*) newEnv);
        if (!reb){
            AntiResult result = {AntiType::AT_HOOK,
                                 ANTI_DEBUG_ERR_HOOK_JNI + 2};
            AntiResultHandler::getInstance()->HandleResult(&result);
        }
        dx_sleep(10);
    }
    return NULL;
}



STEE
int DXAnti_detect_jni_hook_async(pthread_t &tid) {
    int ret = -1;
    dx_anti_pthread_create(ret, &tid, NULL, DXAnti_detect_jni_hook, NULL);
    return ret;
}


STEE
void DXAnti_detect_jni_hook_entry() {
    pthread_t tid;
    DXAnti_detect_jni_hook_async(tid);
}



